#pragma once

#include "IPEvent.hpp"

namespace Capefox {
    class AnomalyEvent : public IPEvent {
        public:
            AnomalyEvent(Record* eventData);
    };
};