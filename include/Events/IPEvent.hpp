#pragma once

#include "Event.hpp"

#include "IP.hpp"

#include <cstdint>

namespace Capefox {
    class IPEvent : public Event {
        protected:
            IP srcIp;
            IP dstIp;

            uint64_t flowID = 0;

            IPEvent(Record* eventData);

        public:
            IP getSrcIp() const { return srcIp; }
            IP getDstIp() const { return dstIp; }

            uint64_t getFlowID() const { return flowID; }
    };
};