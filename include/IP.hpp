#pragma once

#include <string>
#include <cstdint>
#include <iomanip>

namespace Capefox {
    class IP {
        protected:
            uint32_t address = 0;
            uint8_t netmask = 32;

            bool prvAddr = false;

        public:
            IP() = default;
            IP(std::string address);
            IP(std::string address, uint8_t netmask);

            bool isPublic() const { return !prvAddr; }
            bool isPrivate() const { return prvAddr; }

            friend std::ostream& operator<<(std::ostream& os, const IP& other) {
                return os << std::string(other);
            }

            bool operator==(const IP& other) const {
                return (address == other.address);
            }

            bool operator<(const IP& other) const {
                return (address < other.address);
            }

            bool operator>(const IP& other) const {
                return (address > other.address);
            }

            operator std::string() const {
                std::stringstream os;
                const uint8_t* addr = reinterpret_cast<const uint8_t*>(&address);

                #if __BYTE_ORDER__  == __ORDER_BIG_ENDIAN__
                    os << (unsigned)addr[0] << "." << (unsigned)addr[1] << "." << (unsigned)addr[2] << "." << (unsigned)addr[3];
                #else
                    os << (unsigned)addr[3] << "." << (unsigned)addr[2] << "." << (unsigned)addr[1] << "." << (unsigned)addr[0];
                #endif

                if (netmask != 32) {
                    os << "/" << (unsigned)netmask;
                }

                return os.str();
            }
    };
};