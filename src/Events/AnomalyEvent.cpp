#include "Events/AnomalyEvent.hpp"

using namespace Capefox;

AnomalyEvent::AnomalyEvent(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Anomaly;
}
