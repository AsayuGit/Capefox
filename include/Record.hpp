#pragma once

#include <string>
#include <iomanip>

#include <cstdint>

#include "IP.hpp"
#include "EventType.hpp"

namespace Capefox {
    // The records class serves as an asbstraction to the eve.json file and the Json library
    // as this projet is designed to be estensible we don't allow ourselves to propagate
    // a dependency across the codebase.
    class Record {
        public:
            virtual ~Record() = default;

            virtual std::string toString() const = 0;

            virtual Record* valueAt(const char* value) const = 0;
            virtual Record* valueAt(unsigned int index) const = 0;

            // This methods allow us to manipulate the eve file by asking for a value by name
            template<typename T> T valueAs() const;
            template<typename T> T valueAtAs(const char* value) const {
                Record* record = valueAt(value);
                T val = record->valueAs<T>();
                delete record;

                return val;
            }

            friend std::ostream& operator<<(std::ostream& os, Record& other) {
                os << other.toString();
                return os;
            }

        protected:
            // Theses virtual methods implements fetching each supported types
            virtual std::string valueAsString() const = 0;
            virtual uint64_t valueAsInteger() const = 0;
            virtual std::tm valueAsTime() const = 0;
            virtual EventType valueAsEventType() const = 0;
    };
};