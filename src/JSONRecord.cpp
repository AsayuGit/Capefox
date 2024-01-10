#include "JSONRecord.hpp"

#include <cstring>
#include <sstream>

#include <iostream>

using namespace Capefox;

JSONRecord::JSONRecord(nlohmann::json recordData) {
    data = recordData;
}

JSONRecord::JSONRecord(std::string recordData) : JSONRecord(nlohmann::json::parse(recordData)) {}

std::string JSONRecord::toString() const {
    return data.dump();
}

Record* JSONRecord::valueAt(const char* value) const {
    try {
        return new JSONRecord(data.at(value));
    } catch (nlohmann::json_abi_v3_11_2::detail::out_of_range const&) {
        return nullptr;
    }
}

Record* JSONRecord::valueAt(unsigned int index) const {
    try {
        return new JSONRecord(data.at(index));
    } catch (nlohmann::json_abi_v3_11_2::detail::out_of_range const&) {
        return nullptr;
    }
}

std::string JSONRecord::valueAsString() const {
    return std::string(data);
}

uint64_t JSONRecord::valueAsInteger() const {
    return data;
}

std::tm JSONRecord::valueAsTime() const {
    std::tm timestamp;

    std::string timestampStr(data);
    std::istringstream timestampStream(timestampStr);
    timestampStream >> std::get_time(&timestamp, "%Y-%m-%dT%H:%M:%S");

    return timestamp;
}

EventType JSONRecord::valueAsEventType() const {
    std::string eventType(data);

    if (eventType == "alert") {
        return EventType::Alert;
    } else if (eventType == "anomaly") {
        return EventType::Anomaly;
    } else if (eventType == "dcerpc") {
        return EventType::Dcerpc;
    } else if (eventType == "dns") {
        return EventType::Dns;
    } else if (eventType == "dhcp") {
        return EventType::Dhcp;
    } else if (eventType == "fileinfo") {
        return EventType::Fileinfo;
    } else if (eventType == "flow") {
        return EventType::Flow;
    } else if (eventType == "http") {
        return EventType::Http;
    } else if (eventType == "krb5") {
        return EventType::Krb5;
    } else if (eventType == "smb") {
        return EventType::Smb;
    } else if (eventType == "stats") {
        return EventType::Stat;
    } else if (eventType == "tls") {
        return EventType::Tls;
    } else {
        return EventType::Unknown;
    }
}