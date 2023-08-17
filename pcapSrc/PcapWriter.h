//
// Created by Eren on 15.08.2023.
//

#ifndef PARSER_PCAPWRITER_H
#define PARSER_PCAPWRITER_H


#include <fstream>
#include <queue>
#include "../3rdParty/readerwriterqueue.h"

class PcapWriter {


public:
    PcapWriter() = default;

    static void writePackets(moodycamel::ReaderWriterQueue<std::string> &queue,std::ofstream & outfile);



private:

};

#endif //PARSER_PCAPWRITER_H
