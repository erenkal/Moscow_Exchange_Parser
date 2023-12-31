#include <iostream>
#include <fstream>
#include "pcapSrc/PCAPStructs.h"
#include "pcapSrc/PcapReader.h"
#include "pcapSrc/PcapWriter.h"
#include "simbaSrc/SimbaReader.h"
#include "simbaSrc/SimbaWriter.h"
#include <queue>
#include <thread>
#include "3rdParty/NanoLog.h"
#include "3rdParty/readerwriterqueue.h"





enum class ParseType {
    Pcap,
    Simba
};


void startNanoLog (const std::string& outputFile) {
    /* We are using nanolog for simba messages output because it has a non-blocking and lock-free ring buffer implementation (which I
     * find useful for high performance applications) and it is fast.
     * If I couldn't use it I would implement a similar logger with ring buffer and lock-free synchronization.
     *
     * I have changed source code to not write any log type message (like timestamps etc.)
     * It only writes the message itself.
     * */
    nanolog::initialize(nanolog::NonGuaranteedLogger(128), "./", outputFile, 2000);
    nanolog::set_log_level(nanolog::LogLevel::INFO);
    LOG_CRIT << "Starting Simba Parser";
}




int main(int argc, char* argv[]) {
//    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false); //disable synchronization between C and C++ standard streams to speed up I/O

    if (argc < 4) {
    std::cerr << "Please Write: <parse_type> <input_file> <output_file> \n (p for Pcap) (s for Simba)" << std::endl;
    std::exit(1);
    }
    ParseType parseType = argv[1][0] == 'p' ? ParseType::Pcap : ParseType::Simba;
    std::string input_file = argv[2];
    std::string output_file = argv[3];

    std::ifstream inputStream(input_file, std::ios::binary);

    if (!inputStream.is_open()) {
    std::cerr << "Can't open input file: " << input_file << std::endl;
    std::exit(1);
    }
    if (parseType == ParseType::Pcap) {
        std::cout << "Parsing pcap file" << std::endl;
        std::string pcapHeader(24, ' ');
        inputStream.read(pcapHeader.data(), pcapHeader.size());
        std::string_view pcapHeaderString(pcapHeader);
        GlobalPcapHeader globalHeader(pcapHeaderString);
        if (globalHeader.magic_number != 0xA1B2C3D4
            && globalHeader.magic_number != 0XD4C3B2A1
            && globalHeader.magic_number != 0XA1B23C4D
            && globalHeader.magic_number != 0X4D3CB2A1
            )
        {
            std::cerr << "unsupported pcap format" << std::endl;
            return 1;
        }
        std::queue<std::string> packets;
        moodycamel::ReaderWriterQueue<std::string> asyncPackets(200000);
        std::ofstream outputStream(output_file, std::ios::binary);
        auto decoder = std::thread([&](){ //thread for decoding packets and writing them to output file
            PcapWriter::writePackets(asyncPackets, outputStream);
        });
        decoder.detach();

        PcapReader reader(64);
        int i = 0;
        while (reader.ReadToBuffer(inputStream)){
            reader.SplitPacketsFromBuffer(asyncPackets);
            std::cout << i++ << "th buffer read" <<std::endl;
        }
        std::cout << "finished reading" << std::endl;
        asyncPackets.enqueue("EOF");

        while (true){
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    } else {
        std::cout << "Parsing Simba file" << std::endl;
        startNanoLog(output_file);
        std::queue<std::string> packets;
        SimbaReader reader(64);
        int i = 0;
        moodycamel::ReaderWriterQueue<std::string> asyncPackets(200000);
        SimbaWriter writer;
        auto decoder = std::thread([&](){  //thread for decoding packets and writing them to output file
            writer.parsePackets(asyncPackets);
        });
        decoder.detach();
        while (reader.ReadToBuffer(inputStream)){ //read packets from input file and put them in a buffer
            reader.SplitPacketsFromBuffer(asyncPackets); //split packets from buffer and put them in a queue
            std::cout << i++ << "th buffer read" <<std::endl;
        }
        std::cout << "finished reading" << std::endl;
        asyncPackets.enqueue("EOF");
        while (true){
            std::this_thread::sleep_for(std::chrono::seconds(5)); //keep main thread alive
        }

    }















    return 0;
}
