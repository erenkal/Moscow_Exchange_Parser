//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#include <thread>
#include <queue>
#include "SimbaWriter.h"
#include "SimbaStructs.h"
#include "../3rdParty/NanoLog.h"



void SimbaWriter::log_async()
{

    if (!isStart) {
        auto worker = std::thread([&](){
            std::shared_ptr<MarketDataPacket> str;
            while (true){
                if (ended && writtenPacketCount % 100000 == 0 || ended && writtenPacketCount == parsedPacketCount) UNLIKELY {
                    std::cout << "writtenPacketCount: " << writtenPacketCount << " parsedPacketCount: " << parsedPacketCount << std::endl;
                    if(writtenPacketCount == parsedPacketCount)
                        break;
                }
                bool res = mdpQ.try_dequeue(str);
                if (res) {
                    LOG_CRIT << (*str).to_string();
                    writtenPacketCount++;
                }
            }
            std::cout << "all packets are processed. wait for logging to be finished. \n"; //nanolog is async so we need to wait for it to finish
            std::this_thread::sleep_for(std::chrono::seconds(20)); // it takes less but just to be sure we wait 20 seconds
            std::cout << "exiting... \n";
            exit(0);
        });
        worker.detach();
    };
    isStart = true;
}

void SimbaWriter::parsePackets(moodycamel::ReaderWriterQueue<std::string> &queue) {
    log_async();
    std::cout << "Parsing and writing packets to file... \n [" << std::flush;
    uint64_t startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); // simple benchmark tool
    int mod = 100000; // print a loading bar every 100000 packets

    std::string packet;


    while (true) { // this while is parsing the packets from the queue
        bool result = queue.try_dequeue(packet);

         if (!result) UNLIKELY  // if queue is empty continue polling
            continue;
        if (packet == "EOF") UNLIKELY {// a hardcoded EOF packet is sent to the queue to indicate the end of the file
            std::cout << "Found EOF packet, exiting..." << std::endl;
            break;
        }
        if (parsedPacketCount % mod == 0) UNLIKELY  // print a loading bar every 100000 packets
        std::cout << "#" << std::flush;
        std::shared_ptr<MarketDataPacket> mdpPtr = std::make_shared<MarketDataPacket>(packet);
        mdpQ.enqueue(mdpPtr);
        parsedPacketCount++;
    }
    uint64_t endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "]" << std::endl;
    std::cout << "Parsing took " << endTime - startTime << " milliseconds" << std::endl;
    std::cout << "Average packet time: " << (endTime - startTime)*1000000 / parsedPacketCount << " nanoseconds" << std::endl;
    ended = true;
    std::cout << "packet parsed: " << parsedPacketCount << std::endl;



}

std::string_view SimbaWriter::ParsePacket(std::string_view packet , bool isIncremental) { //functiont gets the data starting from the ending of MDP header

    return packet;
}
