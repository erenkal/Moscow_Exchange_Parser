#include <iostream>
#include <fstream>
#include "PCAPStructs.h"
#include "PcapReader.h"
#include "PcapWriter.h"
#include <queue>









int main(int argc, char* argv[]) {
//    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false); //disable synchronization between C and C++ standard streams to speed up I/O

    if (argc < 3) {
    std::cerr << "Please Write: <input_file> <output_file>" << std::endl;
    std::exit(1);
    }
    std::string input_file = argv[1];
    std::string output_file = argv[2];

    std::ifstream inputStream(input_file, std::ios::binary);

    if (!inputStream.is_open()) {
    std::cerr << "Can't open input file: " << input_file << std::endl;
    std::exit(1);
    }
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

    PcapReader reader(64);
    int i = 0;
    while (reader.ReadToBuffer(inputStream)){
        reader.SplitPacketsFromBuffer(packets);
        std::cout << i++ << "th buffer read" <<std::endl;
    }
    std::cout << "finished reading" << std::endl;
    //we should have all the packets in the queue now
    //it is better to use a thread pool here and write to the file in parallel to buffer reading
    //but for now we will just write to the file
    //todo thread pool and sequential blocking writing
    std::ofstream outputStream(output_file, std::ios::binary);
    PcapWriter writer;
    writer.writePackets(packets,outputStream);












    return 0;
}
