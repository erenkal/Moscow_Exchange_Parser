//
// Created by eren ekrem kallıkçı on 14.08.2023.
//

#ifndef PARSER_PCAPREADER_H
#define PARSER_PCAPREADER_H


#include <string>
#include <fstream>

class PcapReader {










private:
    std::string buffer; // buffer to read from
    uint32_t leftBytes; // left bytes in buffer to be read
    uint32_t readBytes; // read bytes from buffer
    std::streamsize readChunkSize; // chunk size to read from file


    void ReadToBuffer(std::ifstream &infile);
};


#endif //PARSER_PCAPREADER_H
