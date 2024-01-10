#include "Events/FlowEvent.hpp"

using namespace Capefox;

FlowEvent::FlowEvent(Record* eventData) : TransportEvent(eventData) {
    eventType = EventType::Flow;

    if (Record* protoData = eventData->valueAt("proto")) {
        proto = protoData->valueAs<std::string>();
        delete protoData;
    }

    if (Record* appProtoData = eventData->valueAt("app_proto")) {
        app_proto = appProtoData->valueAs<std::string>();
        delete appProtoData;
    }
}
