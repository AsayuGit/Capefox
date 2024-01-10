#include "DnsRecord.hpp"

#include <iostream>

using namespace Capefox;

DnsRecord::DnsRecord(Record* recorddata) {
    domain = recorddata->valueAtAs<std::string>("rrname");

    if (Record* addressData = recorddata->valueAt("rdata")) {
        address = addressData->valueAs<IP>();
        delete addressData;
    }
}