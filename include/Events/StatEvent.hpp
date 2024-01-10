#pragma once

#include "Event.hpp"

namespace Capefox {
    class StatEvent : public Event {
        public:
            StatEvent(Record* eventData);
    };
};