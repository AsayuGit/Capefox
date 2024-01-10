#include "Events/StatEvent.hpp"

using namespace Capefox;

StatEvent::StatEvent(Record* eventData) : Event(eventData) {
    eventType = EventType::Stat;
}
