//
// Created by eren ekrem kallıkçı on 14.08.2023.
//

#ifndef PARSER_PCAPREADER_H
#define PARSER_PCAPREADER_H


#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <optional>
#include <span>
#include <iostream>
#include "PCAPStructs.h"

class PcapReader {
public:

    explicit PcapReader(uint32_t readChunkSizeInMB);
    PcapReader(const PcapReader&) = delete;
    PcapReader& operator=(const PcapReader&) = delete;
    PcapReader(PcapReader&&) = delete;
    ~PcapReader() = default;


    bool ReadToBuffer(std::ifstream &infile);

    void SplitPacketsFromBuffer();

    void SplitPacketsFromBuffer(std::queue<std::string> &queue);

private:
    std::string buffer; // buffer to read from
    uint32_t leftBytes{}; // left bytes in buffer to be read
    uint32_t readBytes{}; // read bytes from buffer
    std::streamsize readChunkSize{}; // chunk size to read from file


    static inline std::optional<size_t> GetDatagramLength(std::string_view buf)  {
        if (buf.size() < sizeof(PCAP_LENGTH_HEADER_LENGTH)) return std::nullopt;
        int index = PCAP_LENGTH_INDEX;
        auto length = Parser::parseNumeric<uint32_t>(index, buf);
//        std::cout << "length: " << length << std::endl;
        return length + PCAP_LENGTH_HEADER_LENGTH;
    }
};


#endif //PARSER_PCAPREADER_H
