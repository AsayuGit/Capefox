#include "Events/FileinfoEvent.hpp"

using namespace Capefox;

FileinfoEvent::FileinfoEvent(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Fileinfo;

    Record* fileData = data->valueAt("fileinfo");

    fileName = fileData->valueAtAs<std::string>("filename");
    txID = fileData->valueAtAs<uint64_t>("tx_id");

    if (Record* hash = fileData->valueAt("sha256")) {
        sha256Hash = hash->valueAs<std::string>();
        delete hash;
    }

    if (Record* hash = fileData->valueAt("sha1")) {
        sha1Hash = hash->valueAs<std::string>();
        delete hash;
    }

    if (Record* hash = fileData->valueAt("md5")) {
        md5Hash = hash->valueAs<std::string>();
        delete hash;
    }

    delete fileData;
}
