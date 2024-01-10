#include "Events/DhcpEvent.hpp"

using namespace Capefox;

DhcpEvent::DhcpEvent(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Dhcp;
}
