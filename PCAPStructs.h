//
// Created by eren ekrem kallıkçı on 12.08.2023.
//

#ifndef PARSER_PCAPSTRUCTS_H
#define PARSER_PCAPSTRUCTS_H

#include <cstdint>
#include <string>
#include "Parser.h"

struct GlobalPcapHeader {
    uint32_t magic_number;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t time_zone;
    uint32_t sig_figs;
    uint32_t snap_len;
    uint32_t networkLinkType;

    inline explicit GlobalPcapHeader(std::string_view& rawHeader) {
        int_fast32_t index = 0;
        magic_number = Parser::parseNumeric<decltype(magic_number)>(index, rawHeader);
        version_major = Parser::parseNumeric<decltype(version_major)>(index, rawHeader);
        version_minor = Parser::parseNumeric<decltype(version_minor)>(index, rawHeader);
        time_zone = Parser::parseNumeric<decltype(time_zone)>(index, rawHeader);
        sig_figs = Parser::parseNumeric<decltype(sig_figs)>(index, rawHeader);
        snap_len = Parser::parseNumeric<decltype(snap_len)>(index, rawHeader);
        networkLinkType = Parser::parseNumeric<decltype(networkLinkType)>(index, rawHeader);
    }

    std::string to_string() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const GlobalPcapHeader& header);
};



#endif //PARSER_PCAPSTRUCTS_H
