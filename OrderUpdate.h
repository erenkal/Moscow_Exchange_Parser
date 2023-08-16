//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_ORDERUPDATE_H
#define PARSER_ORDERUPDATE_H


#include <sstream>

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

struct OrderUpdate {
    int64_t  md_entry_id;
    int64_t  md_entry_px;
    int64_t  md_entry_size;
    MDFlags md_flags = MDFlags(0);
    int32_t  security_id;
    uint32_t rpt_seq;
    uint8_t  md_update_action;
    uint8_t  md_entry_type;

    static constexpr size_t SIZE = 42;
    explicit OrderUpdate(std::string_view buffer, int32_t &index);

    static constexpr size_t get_parsed_bytes() { return SIZE; }

    [[nodiscard]] std::string to_string() const {
        std::stringstream ss;
        ss << "Order ID: " << md_entry_id << "\n"
           << "Order price: " << md_entry_px << "\n"
           << "Order volume: " << md_entry_size << "\n"
           << "Order type (bit mask): \n" << md_flags.to_string() << "\n"
           << "Instrument numeric code: " << security_id << "\n"
           << "Incremental refresh sequence number: " << rpt_seq << "\n"
           << "Incremental refresh type: " << md_update_action << "\n"
           << "Record type: " << md_entry_type << "\n";
        return ss.str();
    }


    friend std::ostream& operator<<(std::ostream& os, const OrderUpdate& update_order);
};


#endif //PARSER_ORDERUPDATE_H
