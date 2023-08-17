//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#ifndef PARSER_ORDEREXECUTION_H
#define PARSER_ORDEREXECUTION_H

#include <cstdint>
#include <string>
#include <sstream>

#define INT64_NULL (-9223372036854775807LL - 1)
#define DECIMAL6_NULL (9223372036854775807)


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


struct OrderExecution {
    int64_t md_entry_id;
    int64_t md_entry_px;
    int64_t md_entry_size;
    int64_t last_px;
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
    [[nodiscard]] std::string to_string() const {  //string stream is slower than string concatenation todo: check
        std::stringstream ss;
        ss << "Order ID: " << md_entry_id << "\n"
           << "Order Price: " << md_entry_px << "\n"
           << "Remaining quantity in the order: " << md_entry_size << "\n"
           << "Trade price: " << last_px << "\n"
           << "Trade volume: " << last_qty << "\n"
           << "Trade ID: " << trade_id << "\n"
           << "Trade type (Flags): " << md_flags << "\n"
           << "Instrument numeric code: " << security_id << "\n"
           << "Incremental refresh sequence number: " << rpt_seq << "\n"
           << "Incremental refresh type: " << static_cast<uint8_t>(md_update_action) << "\n"
           << "Record type: " << static_cast<uint8_t>(md_entry_type) << "\n";
        return ss.str();
    }


    friend std::ostream& operator<<(std::ostream& os, const OrderExecution& execution_order);
};


#endif //PARSER_ORDEREXECUTION_H
