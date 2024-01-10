#include "Event.hpp"

#include <iostream>

#include "Events/AlertEvent.hpp"
#include "Events/AnomalyEvent.hpp"
#include "Events/DcerpcEvent.hpp"
#include "Events/DnsEvent.hpp"
#include "Events/DhcpEvent.hpp"
#include "Events/FileinfoEvent.hpp"
#include "Events/FlowEvent.hpp"
#include "Events/HttpEvent.hpp"
#include "Events/IPEvent.hpp"
#include "Events/Krb5Event.hpp"
#include "Events/SmbEvent.hpp"
#include "Events/StatEvent.hpp"
#include "Events/TlsEvent.hpp"

using namespace Capefox;

Event::Event(Record* eventData) {
    data = eventData;
    timestamp = data->valueAtAs<std::tm>("timestamp");
}

// This method is responsible for instianciating the correct event class
// from the event_type field value
Event* Event::New(Record* eventData) {
    Event* newEvent = nullptr;

    // To do so we first get the value from the event_type field
    switch (eventData->valueAtAs<EventType>("event_type")) {
        // Then match on each type to instanciate the right class
        case Alert:
            newEvent = new AlertEvent(eventData);
            break;

        case Anomaly:
            newEvent = new AnomalyEvent(eventData);
            break;

        case Dcerpc:
            newEvent = new DcerpcEvent(eventData);
            break;

        case Dns:
            newEvent = new DnsEvent(eventData);
            break;

        case Dhcp:
            newEvent = new DhcpEvent(eventData);
            break;

        case Fileinfo:
            newEvent = new FileinfoEvent(eventData);
            break;
        
        case Flow:
            newEvent = new FlowEvent(eventData);
            break;
        
        case Http:
            newEvent = new HttpEvent(eventData);
            break;

        case Krb5:
            newEvent = new Krb5Event(eventData);
            break;

        case Smb:
            newEvent = new SmbEvent(eventData);
            break;

        case Stat:
            newEvent = new StatEvent(eventData);
            break;

        case Tls:
            newEvent = new TlsEvent(eventData);
            break;

        // If the right class type can't be found then we instanciate
        // an unknown event.
        default:
            newEvent = new Event(eventData);
            break;
    }

    // Finally we return the newly instanciated event object
    return newEvent;
}

Event::~Event() {
    delete data;
}

// This method converts the C++ tm timestamp into a human readable form
// this is done so we can fetch timestamp in many different format and
// still produce the same output as the timestamp will be converted
// from the native record format to C++'s std::tm and finally back
// to a human readable date format here.
std::string Event::getTimeStamp() const {
    char buffer[100];

    std::strftime(buffer, 100, "%A %d %B %Y %H:%M:%S", &timestamp);
    return buffer;
}