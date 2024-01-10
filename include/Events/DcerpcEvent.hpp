#pragma once

#include "IPEvent.hpp"

namespace Capefox {
    class DcerpcEvent : public IPEvent {
        public:
            DcerpcEvent(Record* eventData);
    };
};