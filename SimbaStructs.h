//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_SIMBASTRUCTS_H
#define PARSER_SIMBASTRUCTS_H

#include <string>
#include "SBEStructs.h"
#include <iostream>

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
            sbe_messages.emplace_back(buf.substr(index));
            index += sbe_messages.back().get_parsed_bytes();
              //check if the order is monostate
            if (std::holds_alternative<std::monostate>(sbe_messages.back().order)) {
//                std::cout << "monostate: " << packet_length <<  std::endl;
                break;
            } else if (std::holds_alternative<OrderUpdate>(sbe_messages.back().order)) {
//                std::cout << "order update" << std::endl;
            } else if (std::holds_alternative<OrderExecution>(sbe_messages.back().order)) {
//                std::cout << "order execution" << std::endl;
            }
        }
        parsed = index;
    }

    size_t get_parsed_bytes() const { return parsed; }
    std::string to_string() const{
        std::string str;
//        str += header.to_string();
        for (const auto& sbe_message : sbe_messages) {
            str += sbe_message.to_string();
        }
        return str;
    };
    friend std::ostream& operator<<(std::ostream& os, const IncrementalPacket& header){
        os << header.to_string();
        return os;
    }
};

struct GroupSize{
    uint16_t block_length = 0;
    uint8_t num_in_group = 0;
    GroupSize() = default;
    GroupSize(std::string_view buf) {
        int index = 0;
        block_length = Parser::parseNumeric<uint16_t>(index, buf);
        num_in_group = Parser::parseNumeric<uint8_t>(index, buf);
    }
};


struct MDEntry{
    int64_t md_entry_id;
    uint64_t transact_time;
    int64_t md_entry_px;
    int64_t md_entry_size;
    int64_t trade_id;
    MDFlags md_flags = MDFlags(0);
    MDFlags md_flags2 = MDFlags(0);
    MDEntryTypeValue md_entry_type; // 0 1 J
    //size
    static constexpr size_t SIZE { 57 };

    MDEntry(std::string_view buf) {
        int index = 0;
        md_entry_id = Parser::parseNumeric<uint64_t>(index, buf);
        transact_time = Parser::parseNumeric<uint64_t>(index, buf);
        md_entry_px = Parser::parseNumeric<uint64_t>(index, buf);
        md_entry_size = Parser::parseNumeric<uint64_t>(index, buf);
        trade_id = Parser::parseNumeric<uint64_t>(index, buf);
        md_flags = Parser::parseNumeric<uint64_t>(index, buf);
        md_flags2 = Parser::parseNumeric<uint64_t>(index, buf);
        md_entry_type = static_cast<MDEntryTypeValue>(Parser::parseNumeric<uint8_t>(index, buf));
    }

    [[nodiscard]] std::string to_string() const {

        std::string str;
        str += "md_entry_id: " + std::to_string(md_entry_id) + "\n";
        str += "transact_time: " + std::to_string(transact_time) + "\n";
        str += "md_entry_px: " + std::to_string(md_entry_px) + "\n";
        str += "md_entry_size: " + std::to_string(md_entry_size) + "\n";
        str += "trade_id: " + std::to_string(trade_id) + "\n";
        str += "md_flags: " + md_flags.to_string()+ "\n";
        str += "md_flags2: " + md_flags2.to_string() + "\n";
        str += &"md_entry_type: " [ static_cast<char>(md_entry_type)];
        str += "\n";
        return str;
    }

    friend std::ostream& operator<<(std::ostream& os, const MDEntry& update_order) {
        os << update_order.to_string();
        return os;
    }



};

// Snapshot Packet
struct SnapshotPacket {

    SbeMessageHeader message_header;
    std::vector<MDEntry> md_messages {};
    int32_t security_id;
    uint32_t last_msg_seq_num_processed;
    uint32_t rpt_seq;
    uint32_t exchange_trading_session_id;
//    GroupSize group_size;
    uint16_t block_len;
    uint8_t num_in_group;

    static constexpr size_t SIZE = 19;


    explicit SnapshotPacket(std::string_view buf, size_t packet_length) : message_header(buf)  {
        if (message_header.template_ID.value != MessageTypeValue::OrderBookSnapshotPacket) {
            return;
        }
        int index = SbeMessageHeader::SIZE;
//        std::cout << "Snapshot packet length: " << packet_length << std::endl;
        security_id = Parser::parseNumeric<uint32_t>(index, buf);
        last_msg_seq_num_processed = Parser::parseNumeric<uint32_t>(index, buf);
        rpt_seq = Parser::parseNumeric<uint32_t>(index, buf);
        exchange_trading_session_id = Parser::parseNumeric<uint32_t>(index, buf);
        block_len = Parser::parseNumeric<uint16_t>(index, buf);
        num_in_group = Parser::parseNumeric<uint8_t>(index, buf);

        for (int i = 0; i < num_in_group; i++) {
            md_messages.emplace_back(buf.substr(index));
            index += MDEntry::SIZE;

        }
    }

    [[nodiscard]] std::string to_string() const {
        std::string str;
//        str += "message_header: " + message_header.to_string() + "\n";
        str += "security_id: " + std::to_string(security_id) + "\n";
        str += "last_msg_seq_num_processed: " + std::to_string(last_msg_seq_num_processed) + "\n";
        str += "rpt_seq: " + std::to_string(rpt_seq) + "\n";
        str += "exchange_trading_session_id: " + std::to_string(exchange_trading_session_id) + "\n";
        for (const auto& md_entry : md_messages) {
            str += md_entry.to_string();
        }
        return str;
    }

    friend std::ostream& operator<<(std::ostream& os, const SnapshotPacket& snapshotPacket){
        os << snapshotPacket.to_string();
        return os;
    }

};


struct MarketDataPacket {
    MarketDataPacketHeader header;
    std::variant<std::monostate, IncrementalPacket, SnapshotPacket> packet;
    explicit MarketDataPacket(std::string_view buf) : header(MarketDataPacketHeader(buf)) {
        std::string_view packetBuffer = buf.substr(MARKET_DATA_PACKET_HEADER_LENGTH);
        if (header.isIncremental())
            packet = IncrementalPacket(packetBuffer, header.msg_size - MARKET_DATA_PACKET_HEADER_LENGTH);
        else {
            packet = SnapshotPacket(packetBuffer, header.msg_size - MARKET_DATA_PACKET_HEADER_LENGTH);
//            std::cout << "Packet: " << std::get<SnapshotPacket>(packet).to_string() << std::endl;
        }
    }

    [[nodiscard]] std::string to_string() const {
        std::string str;
        if (header.isIncremental())
            str += std::get<IncrementalPacket>(packet).to_string();
        else
            str += std::get<SnapshotPacket>(packet).to_string();
        return str;
    }

};




#pragma pack(pop)
#endif //PARSER_SIMBASTRUCTS_H
