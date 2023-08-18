//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_SIMBASTRUCTS_H
#define PARSER_SIMBASTRUCTS_H

#include <string>
#include "SBEStructs.h"
#include <iostream>
#include "SnapShot.h"
#include "OrderUpdate.h"
#include "OrderExecution.h"
#include "../3rdParty/NanoLog.h"


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

struct SbeMessage {
    constexpr static std::size_t SIZE = 8;
    using OrderType = std::variant<std::monostate, OrderUpdate,OrderExecution>;
    SbeMessageHeader header {};
    OrderType order;
    int32_t parsed {};

    explicit SbeMessage(std::string_view buffer):
            header{SbeMessageHeader{buffer}} {
        parsed += SbeMessageHeader::SIZE;
//        std::cout << "template id: " <<(uint16_t) header.template_ID.value << std::endl;
        switch (header.template_ID.value) {  //other types are not implemented
            case MessageTypeValue::OrderUpdate: { order = OrderUpdate{buffer, parsed}; break;}
            case MessageTypeValue::OrderExecution: { order = OrderExecution{buffer,parsed}; break;}
                break;
            default: {
                order = std::monostate{};
                break;
            }
        }
    }

    std::string to_string() const{
        std::string result;
//        result += header.to_string();
        result += std::visit([](auto&& arg) -> std::string {
            if constexpr (!std::is_same_v<std::monostate, std::decay_t<decltype(arg)>>) {
                return arg.to_string();
            } else {
                return "unimplemented message";
            }
        }, order);
        return result;
    };
    size_t get_parsed_bytes() const { return parsed; }
    friend std::ostream& operator<<(std::ostream& os, const SbeMessage& header);
};

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
    [[nodiscard]] std::string to_string() const{
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


struct MarketDataPacket {
    MarketDataPacketHeader header;
    std::variant<std::monostate, IncrementalPacket, SnapshotPacket> packet;
    explicit MarketDataPacket(std::string_view buf) : header(MarketDataPacketHeader(buf)) {
        std::string_view packetBuffer = buf.substr(MARKET_DATA_PACKET_HEADER_LENGTH);
        if (header.isIncremental()) LIKELY {
            packet = IncrementalPacket(packetBuffer, header.msg_size - MARKET_DATA_PACKET_HEADER_LENGTH);
        }
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

    friend std::ostream& operator<<(std::ostream& os, const MarketDataPacket& header){
        os << header.to_string();
        return os;
    }
    friend void operator<<(nanolog::NanoLogLine os, const MarketDataPacket* header){
        os << header->to_string();

    }

};




#endif //PARSER_SIMBASTRUCTS_H
