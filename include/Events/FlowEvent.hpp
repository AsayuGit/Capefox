#pragma once

#include "Events/TransportEvent.hpp"

#include <string>

namespace Capefox {
    class FlowEvent : public TransportEvent {
        private:
            std::string proto;
            std::string app_proto;

        public:
            FlowEvent(Record* eventData);

            std::string getProtocol() const { return proto; }
            std::string getAppProtocol() const { return app_proto; }
    };
};