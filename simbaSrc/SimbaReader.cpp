//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#include <cstdint>
#include "SimbaReader.h"
#include "../pcapSrc/PcapReader.h"
#include "../3rdParty/readerwriterqueue.h"

SimbaReader::SimbaReader(uint32_t readChunkSizeInMB) {
    readChunkSize = readChunkSizeInMB * 1024 * 1024;
    //  buffer.resize(readChunkSize);
    leftBytes = 0;
    readBytes = 0;

}

bool SimbaReader::ReadToBuffer(std::ifstream& infile) {
    // Move the leftover bytes to the start of the buffer
    memmove(buffer.data(), buffer.data() + (buffer.size() - leftBytes), leftBytes);
    // Resize the buffer to accommodate the left bytes and a new chunk of data
    buffer.resize(leftBytes + readChunkSize);
    // Read a new chunk from the file right after the left bytes
    infile.read(buffer.data() + leftBytes, readChunkSize);
    std::streamsize bytes_read = infile.gcount();
    if (bytes_read == 0) {
        buffer.erase(buffer.begin(), buffer.end());
        // No more data to read from file
        return false;
    }
    // Adjust the buffer size based on actual bytes read
    buffer.resize(leftBytes + bytes_read);
    return true;
}

void SimbaReader::SplitPacketsFromBuffer(moodycamel::ReaderWriterQueue<std::string> &queue) {
    size_t curIndex = 0;

    while (curIndex < buffer.size()) {
        std::string_view leftBuffer(buffer.data() + curIndex, buffer.size() - curIndex);
        const auto packetLength = GetDatagramLength(leftBuffer);

        if (!packetLength.has_value() || curIndex + packetLength.value() > buffer.size()) {
            break;
        }

        queue.emplace(leftBuffer.data(), packetLength.value());
        curIndex += packetLength.value();
    }

    //  buffer.erase(buffer.begin(), buffer.begin() + curIndex);

    leftBytes = buffer.size() - curIndex;
}
