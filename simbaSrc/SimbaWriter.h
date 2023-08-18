//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_SIMBAWRITER_H
#define PARSER_SIMBAWRITER_H


#include <queue>
#include <fstream>
#include "../3rdParty/readerwriterqueue.h"
#include "SimbaStructs.h"

class SimbaWriter {

    std::string_view ParsePacket(std::string_view buf, bool isIncremental);

    moodycamel::ReaderWriterQueue<std::shared_ptr<MarketDataPacket>> mdpQ{1000000};


    bool isStart = false; // running check for log_async
    std::atomic<int> writtenPacketCount = 0;
    std::atomic<int> parsedPacketCount = 0;

    std::atomic<bool> ended = false;

public:
    SimbaWriter() = default;
    SimbaWriter(const SimbaWriter&) = delete;
    SimbaWriter& operator=(const SimbaWriter&) = delete;
    SimbaWriter(SimbaWriter&&) = delete;

    void parsePackets(moodycamel::ReaderWriterQueue<std::string> &queue);

    void log_async();
};


#endif //PARSER_SIMBAWRITER_H
