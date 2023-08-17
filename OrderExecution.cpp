#include <cstdint>

//
// Created by eren ekrem kallıkçı on 16.08.2023.
//

#include "OrderExecution.h"
#include "Parser.h"

OrderExecution::OrderExecution(std::string_view buffer, int32_t &index) {

    md_entry_id = Parser::parseNumeric<int64_t>(index, buffer);
    md_entry_px = Parser::parseNumeric<int64_t>(index, buffer);
    md_entry_size = Parser::parseNumeric<int64_t>(index, buffer);
    last_px = Parser::parseNumeric<int64_t>(index, buffer);
    last_qty = Parser::parseNumeric<int64_t>(index, buffer);
    trade_id = Parser::parseNumeric<int64_t>(index, buffer);
    md_flags = Parser::parseNumeric<uint64_t>(index, buffer);
    md_flags2 = Parser::parseNumeric<uint64_t>(index, buffer);
    security_id = Parser::parseNumeric<int32_t>(index, buffer);
    rpt_seq = Parser::parseNumeric<uint32_t>(index, buffer);
    md_update_action = static_cast<MDUpdateActionValue> (Parser::parseNumeric<uint8_t>(index, buffer));
    md_entry_type = static_cast<MDEntryTypeValue> (Parser::parseNumeric<uint8_t>(index, buffer));

}

