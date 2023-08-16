//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#include "SimbaWriter.h"
#include "SimbaStructs.h"

void SimbaWriter::writePackets(std::queue <std::string> queue, std::ofstream &outfile) {
    while (!queue.empty()) {
        std::string_view packet = queue.front();
//        MarketDataPacketHeader header (packet);
//        ParsePacket(packet ,header.isIncremental());
        MarketDataPacket mdp(packet);
        queue.pop();
    }


}

std::string_view SimbaWriter::ParsePacket(std::string_view packet , bool isIncremental) { //functiont gets the data starting from the ending of MDP header

    return packet;



}
