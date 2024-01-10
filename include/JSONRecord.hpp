#pragma once

#include "Record.hpp"

#include <string>
#include <nlohmann/json.hpp>

namespace Capefox {
    // This class implements the Record class for json files
    // This class could be implmemented to supports XML files and others as well.
    class JSONRecord : public Record {
        private:
            nlohmann::json data;

            JSONRecord(nlohmann::json recordData);

        public:
            JSONRecord(std::string recordData);

            std::string toString() const override;

            Record* valueAt(const char* value) const override;
            Record* valueAt(unsigned int index) const override;

        private:
            std::string valueAsString() const override;
            uint64_t valueAsInteger() const override;
            std::tm valueAsTime() const override;
            EventType valueAsEventType() const override;
    };
};