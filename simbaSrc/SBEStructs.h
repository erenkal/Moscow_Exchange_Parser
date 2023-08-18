//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_SBESTRUCTS_H
#define PARSER_SBESTRUCTS_H
#include <cstdint>
#include <string>
#include <stdexcept>
#include "../Parser.h"
#include <iostream>
#include <span>

constexpr int64_t INT64_NULL = -9223372036854775807LL - 1;  //used constexpr because it's safer
constexpr int64_t DECIMAL5_NULL = 9223372036854775807;

enum class Flags : uint64_t {
    DAY = 0x1,
    IOC = 0x2,
    OTC = 0x4,
    END_OF_TRANSACTION = 0x1000,
    FILL_OR_KILL = 0x80000,
    ORDER_MOVE_RESULT = 0x100000,
    CANSEL_RESULT = 0x200000,
    MESS_CANCEL_RESULT = 0x400000,
    NEGOTIATED_ORDER= 0x4000000,
    MULTI_LEG_OREDER= 0x8000000,
    SIGN_OF_ORDER_DELETION_DUE_TO_A_CROSS_TRADE = 0x20000000,
    CANCEL_OF_DISCONNECT_RESULT = 0x100000000,
    SYNTHETIC_ORDER = 0x200000000000,
    RFS_ORDER = 0x400000000000,
};

enum class MDPFlags : uint16_t {
    kLastFragment     = 0x1,
    kFirstSnapshotMsg = 0x2,
    kLastSnapshotMsg  = 0x4,
    kIncremental      = 0x8, //Important
    kPossDupFlag      = 0x10,
};

struct MDFlags {
    uint64_t value;

    MDFlags(uint64_t value): value(value) {}

    bool if_mask_active(Flags flag) const noexcept {
        return (value & static_cast<uint64_t>(flag)) == static_cast<uint64_t>(flag);
    }

    std::string to_string() const {
        std::string ret;

        if (if_mask_active(Flags::DAY)) {
            ret += "* 0x1 Day\n";
        }
        if (if_mask_active(Flags::IOC)) {
            ret += "* 0x2 IOC\n";
        }
        if (if_mask_active(Flags::OTC)) {
            ret += "* 0x4 OTC\n";
        }
        if (if_mask_active(Flags::END_OF_TRANSACTION)) {
            ret += "* 0x1000 - End of transaction bit\n";
        }
        if (if_mask_active(Flags::FILL_OR_KILL)) {
            ret += "* 0x80000 - Fill-or-Kill\n";
        }
        if (if_mask_active(Flags::ORDER_MOVE_RESULT)) {
            ret += "* 0x100000 - The entry is the result of the order move\n";
        }
        if (if_mask_active(Flags::CANSEL_RESULT)) {
            ret += "* 0x200000 - The entry is the result of the order cancel\n";
        }
        if (if_mask_active(Flags::MESS_CANCEL_RESULT)) {
            ret += "* 0x400000 - The entry is the result of the orders mass cancel\n";
        }
        if (if_mask_active(Flags::NEGOTIATED_ORDER)) {
            ret += "* 0x4000000 - Negotiated order\n";
        }
        if (if_mask_active(Flags::MULTI_LEG_OREDER)) {
            ret += "* 0x8000000 - Multi-leg order\n";
        }
        if (if_mask_active(Flags::SIGN_OF_ORDER_DELETION_DUE_TO_A_CROSS_TRADE)) {
            ret += "* 0x20000000 - Sign of order deletion due to a cross-trade\n";
        }
        if (if_mask_active(Flags::CANCEL_OF_DISCONNECT_RESULT)) {
            ret += "* 0x100000000 - The entry is the result of the orders cancel by \"Cancel on Disconnect\" service\n";
        }
        if (if_mask_active(Flags::SYNTHETIC_ORDER)) {
            ret += "* 0x200000000000 - Synthetic order\n";
        }
        if (if_mask_active(Flags::RFS_ORDER)) {
            ret += "* 0x400000000000 - RFS order\n";
        }
        return ret;
    }

    friend std::ostream& operator<<(std::ostream& os, const MDFlags& update_order) {
        os << update_order.to_string();
        return os;
    }
};

struct Decimal5Null {
    uint64_t mantissa{9223372036854775807};
    uint8_t exponent= -5;
    Decimal5Null() = default;
    Decimal5Null(uint64_t mantissa) : mantissa(mantissa) {}
    [[nodiscard]] std::string to_string() const {
        if (mantissa == DECIMAL5_NULL) {
            return "NULL";
        }
        std::string mantissa_str = std::to_string(mantissa);
        return mantissa_str.length() <= 5 ?  //converting mantissa to decimal
               mantissa_str.insert(0, 6 - mantissa_str.length(), '0').insert(1, ".") :
               mantissa_str.insert(mantissa_str.length() - 5, ".");
    }
};


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


enum class MDUpdateActionValue : uint8_t{
    NEW = 0,
    CHANGE = 1,
    DELETE = 2
};
enum class MDEntryTypeValue : uint8_t {
    BID = 48,
    ASK = 49,
    EMPTY_BOOK = 74
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


#endif //PARSER_SBESTRUCTS_H
