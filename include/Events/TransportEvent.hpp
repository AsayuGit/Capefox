#pragma once

#include "IPEvent.hpp"
#include "Port.hpp"

namespace Capefox {
    class TransportEvent : public IPEvent {
        protected:
            Port srcPort;
            Port dstPort;

        public:
            TransportEvent(Record* eventData);

            Port getSrcPort() const { return srcPort; }
            Port getDstPort() const { return dstPort; }
    };
};