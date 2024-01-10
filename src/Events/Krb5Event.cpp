#include "Events/Krb5Event.hpp"

using namespace Capefox;

Krb5Event::Krb5Event(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Krb5;

    Record* krbData = eventData->valueAt("krb5");
    messageType = krbData->valueAtAs<std::string>("msg_type");
    cname = krbData->valueAtAs<std::string>("cname");
    delete krbData;
}
