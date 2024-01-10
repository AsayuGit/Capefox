#include "Events/DnsEvent.hpp"

#include <iostream>

using namespace Capefox;

DnsEvent::DnsEvent(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Dns;

    Record* dnsData = data->valueAt("dns");

    rrname = dnsData->valueAtAs<std::string>("rrname");
    std::string requestType = dnsData->valueAtAs<std::string>("type");
    if (requestType == "answer") {
        type = Answer;

        if (Record* answers = dnsData->valueAt("answers")) {
            // TODO : Iterator
            for (unsigned int i = 0; Record* answer = answers->valueAt(i); ++i) {
                answerRecords.push_back(new DnsRecord(answer));
                delete answer;
            }
            delete answers;
        }
    } else if (requestType == "query") {
        type = Query;
    }

    if (Record* rcodeData = dnsData->valueAt("rcode")) {
        rcode = rcodeData->valueAs<std::string>();
        delete rcodeData;
    }

    if (Record* rrtypeData = dnsData->valueAt("rrtype")) {
        rrtype = rrtypeData->valueAs<std::string>();
        delete rrtypeData;
    }
    
    delete dnsData;
}
