#pragma once

#include "IPEvent.hpp"

#include <string>

namespace Capefox {
    class AlertEvent : public IPEvent {
        private:
            uint64_t signatureID = 0;
            std::string signature = "";
            std::string category = "";

            IP originAddress;
            uint16_t originPort;

            IP targetAddress;
            uint16_t targetPort;
            bool target = false;

            Record* medatata = nullptr;

        public:
            AlertEvent(Record* eventData);
            ~AlertEvent();

            std::string getMetadata(std::string name) const;

            uint64_t getSignatureID() const { return signatureID; }
            std::string getSignature() const { return signature; }
            std::string getCategory() const { return category; }

            IP getOriginAddress() const { return originAddress; }
            uint16_t getOriginPort() const { return originPort; }

            IP getTargetAddress() const { return targetAddress; }
            uint16_t getTargetPort() const { return targetPort; }

            bool hasTarget() const { return target; }
    };
};