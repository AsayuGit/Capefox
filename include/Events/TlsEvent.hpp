#pragma once

#include "IPEvent.hpp"

namespace Capefox {
    class TlsEvent : public IPEvent {
        public:
            TlsEvent(Record* eventData);
    };
};