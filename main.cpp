#include <iostream>
#include <fstream>
#include "PCAPStructs.h"
#include <queue>









int main(int argc, char* argv[]) {

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
        )
    {
        std::cerr << "unsupported pcap format" << std::endl;
        return 1;
    }
    std::queue<std::string > packets;






    return 0;
}
