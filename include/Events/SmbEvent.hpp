#pragma once

#include "IPEvent.hpp"
#include "NTLMSSP.hpp"

#include <string>

namespace Capefox {
    class SmbEvent : public IPEvent {
        private:
            std::string command = "None";
            NTLMSSP* ntlmssp = nullptr;

            std::string clientOS;

        public:
            SmbEvent(Record* eventData);
            ~SmbEvent();

            std::string getCommand() const { return command; }
            NTLMSSP* getNTLMSSP() const { return ntlmssp; }
            std::string getClientOS() const { return clientOS; }
    };
};