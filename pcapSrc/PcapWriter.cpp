//
// Created by Eren on 15.08.2023.
//

#include <iostream>
#include "PcapWriter.h"
#include "../Parser.h"
#include "PCAPStructs.h"

void PcapWriter::writePackets(std::queue<std::string> queue,std::ofstream & outfile) { //parse the pcap strings and write data to file
    std::string output;
    int i = 0;
    std::cout << "Writing to file.. queue size: " << queue.size() << std::endl;
    while (!queue.empty()) {
        i++;
        std::string_view packet = queue.front();
//        int index = PCAP_LENGTH_HEADER_LENGTH + ETHERNET_ETHERTYPE_INDEX ;
//        auto ethernetType = Parser::parseNumeric<uint16_t>(index, packet);
//        if (static_cast<EthernetIpProtocol>(ethernetType) != EthernetIpProtocol::kIPv4) {
//            queue.pop();
//            std::cout << "Found non-IPv4 packet, skipping..." << std::endl;
//            continue;
//        }
//        index += IP_PROTOCOL_INDEX;
//        auto ipProtocol = Parser::parseNumeric<uint8_t>(index, packet); // index is now at the end of IP protocol field
//        index += IP_PROTOCOL_LEFT_SHIFT; // index is now at the end of IP protocol header length field
//
//        if (static_cast<IpProtocol>(ipProtocol) != IpProtocol::kUDP) {
//            std::cout << "Found non-UDP packet, skipping..." << std::endl;
//            queue.pop();
//            continue;
//        }
//        index += UDP_HEADER_LENGTH; // index is now at the end of UDP header length field
//        // index is now at the beginning of UDP payload
        int index = 57;

        std::string_view udpPayload = packet.substr(index+1); // index is +1 because of the 0 indexing of string
//        std::cout << "UDP payload: " << udpPayload << std::endl;

        //a primitive buffered writer
        output.append(udpPayload);
        if (i % 100000 == 0) {
            std::cout << "Writing packet " << i << " to file." << std::endl;
            outfile.write(output.data(), output.size());
            output.clear();
        }
        queue.pop();
//        output.append(udpPayload);
    }
    std::cout << "Finished writing to file." << std::endl;
//    outfile.write(output.data(), output.size());

}
