//
// Created by Eren on 15.08.2023.
//

#ifndef PARSER_PCAPWRITER_H
#define PARSER_PCAPWRITER_H


#include <fstream>
#include <queue>

class PcapWriter {


public:
    PcapWriter() = default;

    void writePackets(std::queue<std::string> queue,std::ofstream & outfile);



private:

};

#endif //PARSER_PCAPWRITER_H
