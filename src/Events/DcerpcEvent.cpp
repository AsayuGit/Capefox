#include "Events/DcerpcEvent.hpp"

using namespace Capefox;

DcerpcEvent::DcerpcEvent(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Dcerpc;
}
