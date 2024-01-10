#include "Events/HttpEvent.hpp"

using namespace Capefox;

HttpEvent::HttpEvent(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Http;
}
