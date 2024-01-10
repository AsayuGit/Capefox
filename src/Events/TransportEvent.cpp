#include "Events/TransportEvent.hpp"

using namespace Capefox;

TransportEvent::TransportEvent(Record* eventData) : IPEvent(eventData) {
    if (Record* portRecord = data->valueAt("src_port")) {
        srcPort = portRecord->valueAs<uint64_t>();
        delete portRecord;
    }

    if (Record* portRecord = data->valueAt("dest_port")) {
        dstPort = portRecord->valueAs<uint64_t>();
        delete portRecord;
    }
}