//
// Created by Eren on 15.08.2023.
//

#include <iostream>
#include "PcapWriter.h"
#include "../Parser.h"
#include "PCAPStructs.h"

constexpr int UDP_PAYLOAD_INDEX = 58;

void PcapWriter::writePackets(moodycamel::ReaderWriterQueue<std::string> &queue,std::ofstream & outfile) { //parse the pcap strings and write data to file
    std::string output;
    std::string packet;
    int i = 0;
    std::cout << "Writing to file.." << std::endl;
    while (true) {
        i++;
        bool result = queue.try_dequeue(packet);
        if (!result) UNLIKELY // if queue is empty continue polling
            continue;
        if (packet == "EOF") UNLIKELY {// a hardcoded EOF packet is sent to the queue to indicate the end of the file
            std::cout << "Found EOF packet, exiting..." << std::endl;
            exit(0);
            break;
        }

        //COMMENTED OUT CODE for finding UDP packets just to show reviewer how I would do it

/*        int index = PCAP_LENGTH_HEADER_LENGTH + ETHERNET_ETHERTYPE_INDEX ;
        auto ethernetType = Parser::parseNumeric<uint16_t>(index, packet);
        if (static_cast<EthernetIpProtocol>(ethernetType) != EthernetIpProtocol::kIPv4) {
            queue.pop();
            std::cout << "Found non-IPv4 packet, skipping..." << std::endl;
            continue;
        }
        index += IP_PROTOCOL_INDEX;
        auto ipProtocol = Parser::parseNumeric<uint8_t>(index, packet); // index is now at the end of IP protocol field
        index += IP_PROTOCOL_LEFT_SHIFT; // index is now at the end of IP protocol header length field

        if (static_cast<IpProtocol>(ipProtocol) != IpProtocol::kUDP) {
            std::cout << "Found non-UDP packet, skipping..." << std::endl;
            queue.pop();
            continue;
        }
        index += UDP_HEADER_LENGTH;
        // index is now at the beginning of UDP payload */

        int index = UDP_PAYLOAD_INDEX; // just using predefined index to make it faster

        //a primitive buffered writer to write to file every 100000 packets
        output.append(packet.substr(index));
        if (i % 100000 == 0) {
            std::cout << "Writing packet " << i << " to file." << std::endl;
            outfile.write(output.data(), output.size());
            output.clear();
        }
    }
    std::cout << "Finished writing to file." << std::endl;
}
