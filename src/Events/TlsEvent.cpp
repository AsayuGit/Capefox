#include "Events/TlsEvent.hpp"

using namespace Capefox;

TlsEvent::TlsEvent(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Tls;
}
