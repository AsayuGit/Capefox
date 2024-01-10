#pragma once

#include "IPEvent.hpp"

namespace Capefox {
    class DhcpEvent : public IPEvent {
        public:
            DhcpEvent(Record* eventData);
    };
};