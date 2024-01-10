#pragma once

#include "IPEvent.hpp"

#include <string>

namespace Capefox {
    class Krb5Event : public IPEvent {
        private:
            std::string messageType = "";
            std::string cname = "";

        public:
            Krb5Event(Record* eventData);

            std::string getMessageType() const { return messageType; }
            std::string getCName() const { return cname; }
    };
};