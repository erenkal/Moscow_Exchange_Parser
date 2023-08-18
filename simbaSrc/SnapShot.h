//
// Created by eren ekrem kallıkçı on 18.08.2023.
//

#ifndef PARSER_SNAPSHOT_H
#define PARSER_SNAPSHOT_H
#include "SbeStructs.h"
#include <string>




struct GroupSize{
    uint16_t block_length = 0;
    uint8_t num_in_group = 0;
    GroupSize() = default;
    GroupSize(std::string_view buf) {
        int index = 0;
        block_length = Parser::parseNumeric<uint16_t>(index, buf);
        num_in_group = Parser::parseNumeric<uint8_t>(index, buf);
    }
};


struct MDEntry{
    int64_t md_entry_id;
    uint64_t transact_time;
    Decimal5Null md_entry_px_mantissa;
    int64_t md_entry_size;
    int64_t trade_id;
    MDFlags md_flags = MDFlags(0);
    MDFlags md_flags2 = MDFlags(0);
    MDEntryTypeValue md_entry_type; // 0 1 J
    //size
    static constexpr size_t SIZE { 57 };

    MDEntry(std::string_view buf) {
        int index = 0;
        md_entry_id = Parser::parseNumeric<int64_t>(index, buf);
        transact_time = Parser::parseNumeric<uint64_t>(index, buf);
        md_entry_px_mantissa = Parser::parseNumeric<uint64_t>(index, buf);
        md_entry_size = Parser::parseNumeric<int64_t>(index, buf);
        trade_id = Parser::parseNumeric<int64_t>(index, buf);
        md_flags = Parser::parseNumeric<uint64_t>(index, buf);
        md_flags2 = Parser::parseNumeric<uint64_t>(index, buf);
        md_entry_type = static_cast<MDEntryTypeValue>(Parser::parseNumeric<uint8_t>(index, buf));
    }

    [[nodiscard]] std::string to_string() const {

        std::string str;
        str.reserve(250);
        str += "\t\t MDEntry\n";
        str += "\t\t Entry Id : " + std::to_string(md_entry_id) + "\n";
        str += "\t\t Transaction Time: " + std::to_string(transact_time) + "\n";
        str += "\t\t Price: " + md_entry_px_mantissa.to_string() + "\n";
        str += "\t\t Entry Size: " + std::to_string(md_entry_size) + "\n";
        str += "\t\t Trade Id: " + std::to_string(trade_id) + "\n";
        str += "\t\t Flags : " + md_flags.to_string()+ "\n";
//        str += "\t\t md_flags2: " + md_flags2.to_string()+ "\n"; // empty
        str += &"\t\t Entry Type: " [ static_cast<char>(md_entry_type)];
        str += "\n";
        return str;
    }

    friend std::ostream& operator<<(std::ostream& os, const MDEntry& update_order) {
        os << update_order.to_string();
        return os;
    }



};

// Snapshot Packet
struct SnapshotPacket {

    SbeMessageHeader message_header;
    std::vector<MDEntry> md_messages {};
    int32_t security_id;
    uint32_t last_msg_seq_num_processed;
    uint32_t rpt_seq;
    uint32_t exchange_trading_session_id;
//    GroupSize group_size;
    uint16_t block_len;
    uint8_t num_in_group;

    static constexpr size_t SIZE = 19;


    explicit SnapshotPacket(std::string_view buf, size_t packet_length) : message_header(buf)  {
        if (message_header.template_ID.value != MessageTypeValue::OrderBookSnapshotPacket) {
            return;
        }
        int index = SbeMessageHeader::SIZE;
//        std::cout << "Snapshot packet length: " << packet_length << std::endl;
        security_id = Parser::parseNumeric<uint32_t>(index, buf);
        last_msg_seq_num_processed = Parser::parseNumeric<uint32_t>(index, buf);
        rpt_seq = Parser::parseNumeric<uint32_t>(index, buf);
        exchange_trading_session_id = Parser::parseNumeric<uint32_t>(index, buf);
        block_len = Parser::parseNumeric<uint16_t>(index, buf);
        num_in_group = Parser::parseNumeric<uint8_t>(index, buf);

        for (int i = 0; i < num_in_group; i++) {
            md_messages.emplace_back(buf.substr(index));
            index += MDEntry::SIZE;

        }
    }
    [[nodiscard]] std::string to_string() const {
        std::string str;
        // str += "Message Header: " + message_header.to_string() + "\n";
        str += "---- SNAPSHOT PACKET ----\n";
        str += "Security ID: " + std::to_string(security_id) + "\n";
        str += "Last Message Sequence Number Processed: " + std::to_string(last_msg_seq_num_processed) + "\n";
        str += "Report Sequence: " + std::to_string(rpt_seq) + "\n";
        str += "Exchange Trading Session ID: " + std::to_string(exchange_trading_session_id) + "\n";
        str += "---- Market Data Entries ----\n";
        for (const auto& md_entry : md_messages) {
            str += md_entry.to_string();
        }
        str += "---- End of Snapshot Packet ----\n";
        return str;
    }

    friend std::ostream& operator<<(std::ostream& os, const SnapshotPacket& snapshotPacket){
        os << snapshotPacket.to_string();
        return os;
    }

};



#endif //PARSER_SNAPSHOT_H
