#include "Events/AlertEvent.hpp"

using namespace Capefox;

AlertEvent::AlertEvent(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Alert;

    Record* alertData = eventData->valueAt("alert");
    signatureID = alertData->valueAtAs<uint64_t>("signature_id");
    signature = alertData->valueAtAs<std::string>("signature");
    category = alertData->valueAtAs<std::string>("category");

    if (Record* orgData = alertData->valueAt("source")) {
        originAddress = orgData->valueAtAs<IP>("ip");
        originPort = orgData->valueAtAs<uint64_t>("port");
        delete orgData;
    }

    if (Record* tgtData = alertData->valueAt("target")) {
        targetAddress = tgtData->valueAtAs<IP>("ip");
        targetPort = tgtData->valueAtAs<uint64_t>("port");
        target = true;
        delete tgtData;
    }

    medatata = alertData->valueAt("metadata");

    delete alertData;
}

AlertEvent::~AlertEvent() {
    if (medatata) {
        delete medatata;
    }
}

std::string AlertEvent::getMetadata(std::string name) const {
    std::string metaStr;
    
    if (medatata) {
        if (Record* metaRecord = medatata->valueAt(name.c_str())) {
            metaStr = metaRecord->toString();
            delete metaRecord;
        }
    }
    return metaStr;
}