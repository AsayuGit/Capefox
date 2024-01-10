#pragma once

#include <string>
#include <iomanip>

namespace Capefox {
    class Domain {
        public:
            enum DomainType {
                Unknown,
                ADDomain,
                ADDomainDC,
                ADDomainPDC
            };

        private:
            std::string name;
            DomainType type = Unknown;

        public:
            Domain() = default;
            Domain(std::string name);

            std::string getName() const { return name; }
            DomainType getType() const { return type; }

            friend std::ostream& operator<<(std::ostream& os, const Domain& other) {
                os << other.name;

                return os;
            }

            bool operator== (const Domain& other) const {
                return name == other.name;
            }

            operator std::string() const {
                return name;
            }
    };
}