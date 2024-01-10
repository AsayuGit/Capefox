#pragma once

#include "Record.hpp"
#include "EventType.hpp"

namespace Capefox {
    class Event {
        protected:
            EventType eventType = Unknown;
            Record* data;
            std::tm timestamp;

            Event(Record* eventData);

        public:
            static Event* New(Record* eventData);
            ~Event();

            EventType getEventType() const { return eventType; }
            std::string getTimeStamp() const;
    };
};