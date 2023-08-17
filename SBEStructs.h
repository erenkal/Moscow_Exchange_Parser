//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_SBESTRUCTS_H
#define PARSER_SBESTRUCTS_H
#include <cstdint>
#include <string>
#include <stdexcept>
#include "OrderUpdate.h"
#include "Parser.h"
#include "OrderExecution.h"
#include <iostream>
#include <span>


enum class MessageTypeValue {
    Heartbeat = 1,
    SequenceReset = 2,
    OrderBestPrices = 3,
    EmptyBook = 4,
    OrderUpdate = 15,
    OrderExecution = 16,
    OrderBookSnapshotPacket = 17,
    SecurityDefinition = 8,
    SecurityStatus = 9,
    SecurityDefinitionUpdateReport = 10,
    TradingSessionStatus = 11,
    Logon = 1000,
    Logout = 1001,
    MarketDataRequest = 1002,
    kUnknown = 0
};





struct TemplateId {
    MessageTypeValue value;

    TemplateId() {}

    explicit TemplateId(uint32_t type) {
        switch(type) {
            case 1: { value = MessageTypeValue::Heartbeat; break;}
            case 2: { value = MessageTypeValue::SequenceReset; break;}
            case 3: { value = MessageTypeValue::OrderBestPrices; break;}
            case 4: { value = MessageTypeValue::EmptyBook; break;}
            case 15: { value = MessageTypeValue::OrderUpdate; break;}  //important
            case 16: { value = MessageTypeValue::OrderExecution; break;} //important
            case 17: { value = MessageTypeValue::OrderBookSnapshotPacket; break;} //important
            case 8: { value = MessageTypeValue::SecurityDefinition; break;}
            case 9: { value = MessageTypeValue::SecurityStatus; break;}
            case 10: { value = MessageTypeValue::SecurityDefinitionUpdateReport; break;}
            case 11: { value = MessageTypeValue::TradingSessionStatus; break;}
            case 1000: { value = MessageTypeValue::Logon; break;}
            case 1001: { value = MessageTypeValue::Logout; break;}
            case 1002: { value = MessageTypeValue::MarketDataRequest; break;}
            default: { value = MessageTypeValue::kUnknown; break;}
        }
    }

    std::string to_string() const {
        switch(value) {
            case MessageTypeValue::Heartbeat: return "Heartbeat";
            case MessageTypeValue::SequenceReset: return "SequenceReset";
            case MessageTypeValue::OrderBestPrices: return "OrderBestPrices";
            case MessageTypeValue::EmptyBook: return "EmptyBook";
            case MessageTypeValue::OrderUpdate: return "OrderUpdate";
            case MessageTypeValue::OrderExecution: return "OrderExecution";
            case MessageTypeValue::OrderBookSnapshotPacket: return "OrderBookSnapshotPacket";
            case MessageTypeValue::SecurityDefinition: return "SecurityDefinition";
            case MessageTypeValue::SecurityStatus: return "SecurityStatus";
            case MessageTypeValue::SecurityDefinitionUpdateReport: return "SecurityDefinitionUpdateReport";
            case MessageTypeValue::TradingSessionStatus: return "TradingSessionStatus";
            case MessageTypeValue::Logon: return "Logon";
            case MessageTypeValue::Logout: return "Logout";
            case MessageTypeValue::MarketDataRequest: return "MarketDataRequest";
            default: throw std::runtime_error("Unsupported sbe message type");
        }
    }


    friend std::ostream& operator<<(std::ostream& os, const TemplateId& message) {
        os << message.to_string();
        return os;
    }
};

struct SbeMessageHeader {
    SbeMessageHeader();

    constexpr static std::size_t SIZE = 8;

    uint16_t block_length {};
    TemplateId template_ID {};
    uint16_t schema_ID {};
    uint16_t version {};

    explicit SbeMessageHeader(std::string_view buffer){
        std::string buffer_copy{buffer};
        std::span<char> span{buffer_copy};
        int index = 0;
        block_length = Parser::parseNumeric<uint16_t>(index, buffer);
        template_ID = TemplateId{(Parser::parseNumeric<uint16_t>(index, buffer))};
        schema_ID = Parser::parseNumeric<uint16_t>(index, buffer);
        version = Parser::parseNumeric<uint16_t>(index, buffer);
    }

    std::string to_string() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const SbeMessageHeader& header);
};

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

        switch (header.template_ID.value) {
//            case MessageTypeValue::OrderBestPrices: { order = BestPricesOrder{reader}; break;}
            case MessageTypeValue::OrderUpdate: { order = OrderUpdate{buffer, parsed}; break;}
            case MessageTypeValue::OrderExecution: { order = OrderExecution{buffer,parsed}; break;}
//            case MessageTypeValue::OrderBookSnapshotPacket: { order = OrderBookSnapshotPacket{reader}; break;} //todo add other types
                break;
            default: {
                order = std::monostate{};
                break;
            }
        }

//        parsed += std::visit([this, &reader]<typename T>(T&& arg) -> uint32_t {
//            if constexpr (!std::is_same_v<std::monostate, std::decay_t<T>>) {
//                return arg.get_parsed_bytes();
//            } else {
//                reader.skip(header.block_length);
//                return header.block_length;
//            }
//        }, order);
    }

    std::string to_string() const;
    size_t get_parsed_bytes() const { return parsed; }

    friend std::ostream& operator<<(std::ostream& os, const SbeMessage& header);
};
#endif //PARSER_SBESTRUCTS_H
