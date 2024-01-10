#include "IP.hpp"

#include <regex>

#include <iostream>

using namespace Capefox;

IP::IP(std::string address) {
    //std::regex ipRegex("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    // To parse an ip address we match on each octet of the address
    std::regex ipRegex("[0-9]{1,3}"); // std::regex ipRegex("(\\d{1,3})");

    auto matchBegin = std::sregex_iterator(address.begin(), address.end(), ipRegex);
    auto matchEnd = std::sregex_iterator();    
    
    // Then for each match
    uint8_t shift = 24;
    for (std::sregex_iterator i = matchBegin; i != matchEnd; ++i) {
        std::smatch match = *i;
        // We append it to the address value
        this->address |= (uint8_t)std::atoi(match.str().c_str()) << shift;
        shift -= 8;
    }

    // Since netmasks are not transmited over the networkd we need
    // to guess them. If the ip is private then its trivial since
    // we can compare the address the the private ip ranges and 
    // infer the relevant mask
    if ((this->address >> 24) == 0xA) { // 10.X.X.X
        prvAddr = true;
        netmask = 8;
    } else if ((this->address >> 20) == 0xAC1) { // 172.16.X.X
        prvAddr = true;
        netmask = 12;
    } else if ((this->address >> 16) == 0xC0A8) { // 192.168.X.X
        prvAddr = true;
        netmask = 16;
    }
}

IP::IP(std::string address, uint8_t netmask) : IP(address) {
    this->netmask = netmask;
    if (this->netmask > 32) {
        this->netmask = 32;
    }
}