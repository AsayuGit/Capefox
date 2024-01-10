#pragma once

#include "IPEvent.hpp"
#include "Domain.hpp"
#include "DnsRecord.hpp"

#include <vector>

namespace Capefox {
    class DnsEvent : public IPEvent {
        public:
            enum DNSRequestType {
                Query,
                Answer
            };

        private:
            Domain rrname;
            std::string rcode;
            std::string rrtype;
            DNSRequestType type;

            std::vector<DnsRecord*> answerRecords;

        public:
            DnsEvent(Record* eventData);

            Domain getRRName() const { return rrname; }
            std::string getRCode() const { return rcode; }
            std::string getRRType() const { return rrtype; }

            DNSRequestType getRequestType() const { return type; }
            const std::vector<DnsRecord*>& getAnswerRecords() const { return answerRecords; }
    };
};