#pragma once

#include "Record.hpp"
#include "Domain.hpp"
#include "IP.hpp"

namespace Capefox {
    class DnsRecord {
        private:
            Domain domain;
            IP address;

        public:
            DnsRecord(Record* recorddata);

            Domain getDomain() const { return domain; }
            IP getAddress() const { return address; }
    };
};