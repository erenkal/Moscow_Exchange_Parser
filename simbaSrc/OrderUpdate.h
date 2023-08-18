//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_ORDERUPDATE_H
#define PARSER_ORDERUPDATE_H


#include <sstream>
#include "SBEStructs.h"



struct OrderUpdate {
    int64_t  md_entry_id;
    Decimal5Null  md_entry_px;
    int64_t  md_entry_size;
    MDFlags md_flags = MDFlags(0);
    MDFlags md_flags2 = MDFlags(0);
    int32_t  security_id;
    uint32_t rpt_seq;
    MDUpdateActionValue  md_update_action;
    MDEntryTypeValue  md_entry_type;

    static constexpr size_t SIZE = 50;
    explicit OrderUpdate(std::string_view buffer, int32_t &index);

    static constexpr size_t get_parsed_bytes() { return SIZE; }

    [[nodiscard]] std::string to_string() const {
        std::string str;
        str.reserve(250);
        str += "------ Order Update ------\n";

        str += "Order ID: " + std::to_string(md_entry_id) + "\n";
        str += "Order price: " + md_entry_px.to_string() + "\n";
        str += "Order volume: " + std::to_string(md_entry_size) + "\n";
        str += "Order type (bit mask): \n" + md_flags.to_string() + "\n";
        str += "Instrument numeric code: " + std::to_string(security_id) + "\n";
        str += "Incremental refresh sequence number: " + std::to_string(rpt_seq) + "\n";
        str += "Type : " + MDUpdateActionValueToString(md_update_action) + "\n";
        str += "Entry Type: " + MDEntryTypeValueToString(md_entry_type); ;
        str += "\n";


        str += "-----------------------------\n";

        return str;
    }


    friend std::ostream& operator<<(std::ostream& os, const OrderUpdate& update_order);
};


#endif //PARSER_ORDERUPDATE_H
