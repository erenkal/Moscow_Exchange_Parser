//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_SIMBASTRUCTS_H
#define PARSER_SIMBASTRUCTS_H

#include <string>
#include "SBEStructs.h"

//TODO: pragma pack(1) for all structs
#pragma pack(push, 1)

enum class MDPFlags : uint16_t {
    kLastFragment     = 0x1,
    kFirstSnapshotMsg = 0x2,
    kLastSnapshotMsg  = 0x4,
    kIncremental      = 0x8, //Important
    kPossDupFlag      = 0x10,
};
// Market Data Packet Header
struct MarketDataPacketHeader {
    uint32_t msg_seq_num;
    uint16_t msg_size;
    uint16_t msg_flags;
    uint64_t sending_time;
    [[nodiscard]] bool isIncremental() const {
        return msg_flags & static_cast<uint16_t>(MDPFlags::kIncremental);
    }

    MarketDataPacketHeader(std::string_view buf ){
        int index = 0;
        msg_seq_num = Parser::parseNumeric<uint32_t>(index, buf);
        msg_size = Parser::parseNumeric<uint16_t>(index, buf);
        msg_flags = Parser::parseNumeric<uint16_t>(index, buf);
        sending_time = Parser::parseNumeric<uint64_t>(index, buf);

    }

};
constexpr int MARKET_DATA_PACKET_HEADER_LENGTH = sizeof(MarketDataPacketHeader);
constexpr int MARKET_DATA_PACKET_HEADER_SIZE_INDEX = 4;

// Incremental Packet
struct IncrementalPacketHeader {
    uint64_t transaction_time;
    uint32_t exchange_trading_session_id;

    static constexpr size_t SIZE {12};

    explicit IncrementalPacketHeader(std::string_view buf) {
        int index = 0;
        transaction_time = Parser::parseNumeric<uint64_t>(index, buf);
        exchange_trading_session_id = Parser::parseNumeric<uint32_t>(index, buf);
    }

    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const IncrementalPacketHeader& header);
};

struct IncrementalPacket {

    IncrementalPacketHeader header;
    std::vector<SbeMessage> sbe_messages {};

    size_t parsed = 0;

    IncrementalPacket(std::string_view buf, size_t packet_length) : header(buf){
        int index = IncrementalPacketHeader::SIZE;
        while (index < packet_length) {
//            sbe_messages.emplace_back(buf.substr(index), packet_length - index);
            index += sbe_messages.back().get_parsed_bytes();
        }
        parsed = index;
    }

    size_t get_parsed_bytes() const { return parsed; }
    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const IncrementalPacket& header);
};

// Snapshot Packet
struct SnapshotPacket {

    SbeMessage sbe_message;

    explicit SnapshotPacket(std::string_view buf, size_t packet_length);

    std::string to_string() const;
    size_t get_parsed_bytes() const { return sbe_message.get_parsed_bytes(); }
    friend std::ostream& operator<<(std::ostream& os, const SnapshotPacket& header);

};


struct MarketDataPacket {
    MarketDataPacketHeader header;
    std::variant<std::monostate, IncrementalPacket, SnapshotPacket> packet;
    explicit MarketDataPacket(std::string_view buf) : header(MarketDataPacketHeader(buf)) {
        std::string_view packetBuffer = buf.substr(MARKET_DATA_PACKET_HEADER_LENGTH);
        if (header.isIncremental())
            packet = IncrementalPacket(packetBuffer, header.msg_size - MARKET_DATA_PACKET_HEADER_LENGTH);
//        else
//            packet = SnapshotPacket(packetBuffer, header.msg_size - MARKET_DATA_PACKET_HEADER_LENGTH);
    }

};




#pragma pack(pop)
#endif //PARSER_SIMBASTRUCTS_H
