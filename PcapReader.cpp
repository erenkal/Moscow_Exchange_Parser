//
// Created by eren ekrem kallıkçı on 14.08.2023.
//

#include "PcapReader.h"
#include <fstream>

void PcapReader::ReadToBuffer(std::ifstream& infile) {
    size_t old_size = buffer.size();
    buffer.resize(old_size + readChunkSize);
    infile.read(buffer.data() + old_size, readChunkSize);
    std::streamsize bytes_read = infile.gcount();
    buffer.resize(old_size + bytes_read);
}



