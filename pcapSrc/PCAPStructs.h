//
// Created by eren ekrem kallıkçı on 12.08.2023.
//

#ifndef PARSER_PCAPSTRUCTS_H
#define PARSER_PCAPSTRUCTS_H

#include <cstdint>
#include <string>
#include "../Parser.h"

struct GlobalPcapHeader {
    uint32_t magic_number;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t time_zone;
    uint32_t sig_figs;
    uint32_t snap_len;
    uint32_t networkLinkType;

    inline explicit GlobalPcapHeader(std::string_view& rawHeader) {
        int_fast32_t index = 0;
        magic_number = Parser::parseNumeric<decltype(magic_number)>(index, rawHeader);
        version_major = Parser::parseNumeric<decltype(version_major)>(index, rawHeader);
        version_minor = Parser::parseNumeric<decltype(version_minor)>(index, rawHeader);
        time_zone = Parser::parseNumeric<decltype(time_zone)>(index, rawHeader);
        sig_figs = Parser::parseNumeric<decltype(sig_figs)>(index, rawHeader);
        snap_len = Parser::parseNumeric<decltype(snap_len)>(index, rawHeader);
        networkLinkType = Parser::parseNumeric<decltype(networkLinkType)>(index, rawHeader);
    }

    std::string to_string() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const GlobalPcapHeader& header);
};

struct PcapLengthHeader {
    uint32_t timestamp_seconds;
    uint32_t timestamp_microseconds;
    uint32_t message_length;
    uint32_t message_length_original;
};

struct EthernetHdr {
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype; //important
};

struct IpHdr {
    uint8_t version_and_header_length;
    uint8_t differentiated_services;
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags_and_fragment_offset;
    uint8_t ttl;
    uint8_t protocol; //important
    uint16_t header_checksum;
    uint32_t source_ip;
    uint32_t dest_ip;
};


struct UdpHdr {
    uint16_t src_port;
    uint16_t dest_port;
    uint16_t length;
    uint16_t checksum;
};





//GLOBAL LENGTHS
constexpr int GLOBAL_PCAP_HEADER_LENGTH = 24;
constexpr int PCAP_LENGTH_HEADER_LENGTH = 16;
constexpr int PCAP_LENGTH_INDEX = 8;
constexpr int ETHERNET_HEADER_LENGTH = 14;
constexpr int ETHERNET_ETHERTYPE_INDEX = 12;
constexpr int IP_PROTOCOL_LENGTH = 20;
constexpr int IP_PROTOCOL_INDEX = 9;
constexpr int IP_PROTOCOL_LEFT_SHIFT = 9;
constexpr int UDP_HEADER_LENGTH = 8;


enum class EthernetIpProtocol : uint16_t {
    kIPv4 = 8,
    kARP = 0x0806,
    kIPv6 = 0x86DD,
    kVLAN = 0x8100,
};

enum class IpProtocol : uint8_t {
    kTCP = 6,
    kUDP = 17,
};



#endif //PARSER_PCAPSTRUCTS_H
