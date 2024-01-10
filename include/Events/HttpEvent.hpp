#pragma once

#include "IPEvent.hpp"

namespace Capefox {
    class HttpEvent : public IPEvent {
        public:
            HttpEvent(Record* eventData);
    };
};