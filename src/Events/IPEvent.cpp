#include "Events/IPEvent.hpp"

#include <iostream>

using namespace Capefox;

IPEvent::IPEvent(Record* eventData) : Event(eventData) {
    srcIp = data->valueAtAs<IP>("src_ip");
    dstIp = data->valueAtAs<IP>("dest_ip");

    flowID = data->valueAtAs<uint64_t>("flow_id");
}
