//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_ORDEREXECUTION_H
#define PARSER_ORDEREXECUTION_H

#include <cstdint>
#include <string>
#include <sstream>
#include "SBEStructs.h"

struct OrderExecution {
    int64_t md_entry_id;
    Decimal5Null md_entry_px;
    int64_t md_entry_size;
    Decimal5Null last_px;
    int64_t last_qty;
    int64_t trade_id;
    int64_t md_flags;
    int64_t md_flags2;
    int32_t security_id;
    uint32_t rpt_seq;
    MDUpdateActionValue md_update_action;
    MDEntryTypeValue md_entry_type;

    static constexpr size_t SIZE = 66;

    explicit OrderExecution(std::string_view buffer, int32_t &index);

    static size_t get_parsed_bytes() {return SIZE;}
    [[nodiscard]] std::string to_string() const {
        std::string str;
        str.reserve(250);
        str += "------ Order Execution ------\n";
        str += "Order ID: " + std::to_string(md_entry_id) + "\n";
        str += "Order Price: " + md_entry_px.to_string() + "\n";
        str += "Remaining quantity in the order: " + std::to_string(md_entry_size == INT64_NULL ? 0 : md_entry_size  ) + "\n";
        str += "Trade price: " + last_px.to_string() + "\n";
        str += "Trade volume: " + std::to_string(last_qty) + "\n";
        str += "Trade ID: " + std::to_string(trade_id) + "\n";
        str += "Trade type (Flags): " + std::to_string(md_flags) + "\n";
        str += "Security ID: " + std::to_string(security_id) + "\n";
        str += "Incremental refresh sequence number: " + std::to_string(rpt_seq) + "\n";
        str += "Type : " + MDUpdateActionValueToString(md_update_action) + "\n";
        str += "Entry Type: " + MDEntryTypeValueToString(md_entry_type); ;
        str += "\n";

        str += "-----------------------------\n";
        return str;
    }

    friend std::ostream& operator<<(std::ostream& os, const OrderExecution& execution_order);
};


#endif //PARSER_ORDEREXECUTION_H
