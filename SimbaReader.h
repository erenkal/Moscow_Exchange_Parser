//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_SIMBAREADER_H
#define PARSER_SIMBAREADER_H
#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <queue>
#include "SimbaStructs.h"
#include "Parser.h"
#include <iostream>


class SimbaReader {
public:
    explicit SimbaReader(uint32_t readChunkSizeInMB);
    SimbaReader(const SimbaReader&) = delete;
    SimbaReader& operator=(const SimbaReader&) = delete;
    SimbaReader(SimbaReader&&) = delete;
    void SplitPacketsFromBuffer(std::__1::queue<std::string> &queue);

    static inline std::optional<size_t> GetDatagramLength(std::string_view buf)  {
        if (buf.size() < sizeof(MarketDataPacketHeader)) return std::nullopt;
        int index = MARKET_DATA_PACKET_HEADER_SIZE_INDEX;
        MarketDataPacketHeader header(buf);
        auto length = header.msg_size;
//        auto length = Parser::parseNumeric<uint16_t>(index, buf);
//        std::cout << "length: " << length << std::endl;
        return length;
    }


    bool ReadToBuffer(std::ifstream &infile);

private:
    std::string buffer; // buffer to read from
    uint32_t leftBytes{}; // left bytes in buffer to be read
    uint32_t readBytes{}; // read bytes from buffer
    std::streamsize readChunkSize{}; // chunk size to read from file

};


#endif //PARSER_SIMBAREADER_H
