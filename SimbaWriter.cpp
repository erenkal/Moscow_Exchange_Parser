//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#include "SimbaWriter.h"
#include "SimbaStructs.h"
#include "3rdParty/NanoLog.h"




void SimbaWriter::writePackets(moodycamel::ReaderWriterQueue<std::string> &queue) {
    std::cout << "Writing packets to file... \n [" << std::flush;
    uint64_t startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); // simple benchmark tool
    int mod = 100000; // print a loading bar every 100000 packets
    int packetCount = 0;
    while (true) {
        std::string packet;
        bool result = queue.try_dequeue(packet);

         if (!result) UNLIKELY  // if queue is empty continue polling
            continue;
         if (packet == "EOF") UNLIKELY  // a hardcoded EOF packet is sent to the queue to indicate the end of the file
            break;
        if (packetCount % mod == 0) UNLIKELY  // print a loading bar every 100000 packets
            std::cout << "#" << std::flush;


        MarketDataPacket mdp(packet);
        LOG_CRIT << (mdp.to_string());

        packetCount++;
    }
    uint64_t endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "]" << std::endl;
    std::cout << "Writing took " << endTime - startTime << " milliseconds" << std::endl;
    std::cout << "Average packet time: " << (endTime - startTime)*1000 / packetCount << std::endl;


}

std::string_view SimbaWriter::ParsePacket(std::string_view packet , bool isIncremental) { //functiont gets the data starting from the ending of MDP header

    return packet;
}
