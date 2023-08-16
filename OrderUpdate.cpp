//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#include <ostream>
#include "OrderUpdate.h"
#include "Parser.h"

OrderUpdate::OrderUpdate(std::string_view buffer, int32_t &index) {
    md_entry_id = Parser::parseNumeric<int64_t>(index, buffer);
    md_entry_px = Parser::parseNumeric<int64_t>(index, buffer);
    md_entry_size = Parser::parseNumeric<int64_t>(index, buffer);
    md_flags = Parser::parseNumeric<uint64_t>(index, buffer);
    security_id = Parser::parseNumeric<int32_t>(index, buffer);
    rpt_seq = Parser::parseNumeric<uint32_t>(index, buffer);
    md_update_action = Parser::parseNumeric<uint8_t>(index, buffer);
    md_entry_type = Parser::parseNumeric<uint8_t>(index, buffer);


}

std::ostream &operator<<(std::ostream &os, const OrderUpdate &update_order) {
    return os << update_order.to_string();
}
