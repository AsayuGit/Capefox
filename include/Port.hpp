#pragma once

#include <cstdint>
#include <string>

namespace Capefox {
    class Port {
        private:
            uint16_t portNB = 0;

        public:
            Port() = default;
            Port(uint16_t portNB);

            std::string getService() const;

            bool operator==(const Port& other) const {
                return (portNB == other.portNB);
            }

            bool operator<(const Port& other) const {
                return (portNB < other.portNB);
            }

            bool operator>(const Port& other) const {
                return (portNB > other.portNB);
            }
    };
};