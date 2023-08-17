//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#include "SimbaWriter.h"
#include "SimbaStructs.h"
#include "3rdParty/NanoLog.h"

void SimbaWriter::writePackets(std::queue <std::string> queue, std::ofstream &outfile) {
    std::cout << "Writing packets to file... \n [" << std::flush;
    uint64_t startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); // simple benchmark tool
    int mod = queue.size()/20;
    int queueSize = queue.size();
    while (!queue.empty()) {
        std::string_view packet = queue.front();
//        MarketDataPacketHeader header (packet);
//        ParsePacket(packet ,header.isIncremental());
        MarketDataPacket mdp(packet);
        queue.pop();
        //cout a loading bar
        if (queue.size() % mod == 0) {
            std::cout << "#" << std::flush;
        }
        LOG_CRIT << (mdp.to_string());
    }
    uint64_t endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "]" << std::endl;
    std::cout << "Writing took " << endTime - startTime << " milliseconds" << std::endl;
    std::cout << "Average packet time: " << (endTime - startTime)*1000 / queueSize << std::endl;


}

std::string_view SimbaWriter::ParsePacket(std::string_view packet , bool isIncremental) { //functiont gets the data starting from the ending of MDP header

    return packet;
}
