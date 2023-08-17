//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_SIMBAWRITER_H
#define PARSER_SIMBAWRITER_H


#include <queue>
#include <fstream>
#include "3rdParty/readerwriterqueue.h"

class SimbaWriter {

    std::string_view ParsePacket(std::string_view buf);

    std::string_view ParsePacket(std::string_view buf, bool isIncremental);

public:
    SimbaWriter() = default;
    SimbaWriter(const SimbaWriter&) = delete;
    SimbaWriter& operator=(const SimbaWriter&) = delete;
    SimbaWriter(SimbaWriter&&) = delete;

    static void writePackets(moodycamel::ReaderWriterQueue<std::string> &queue);
};


#endif //PARSER_SIMBAWRITER_H
