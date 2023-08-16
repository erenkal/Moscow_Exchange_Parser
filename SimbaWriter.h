//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_SIMBAWRITER_H
#define PARSER_SIMBAWRITER_H


#include <queue>
#include <fstream>

class SimbaWriter {

    std::string_view ParsePacket(std::string_view buf);

    std::string_view ParsePacket(std::string_view buf, bool isIncremental);

public:
    SimbaWriter() = default;
    SimbaWriter(const SimbaWriter&) = delete;
    SimbaWriter& operator=(const SimbaWriter&) = delete;
    SimbaWriter(SimbaWriter&&) = delete;

    void writePackets(std::queue<std::string> queue,std::ofstream & outfile);
};


#endif //PARSER_SIMBAWRITER_H
