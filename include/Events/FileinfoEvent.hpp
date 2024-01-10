#pragma once

#include "IPEvent.hpp"

#include <string>

namespace Capefox {
    class FileinfoEvent : public IPEvent {
        private:
            std::string fileName;

            std::string sha256Hash;
            std::string sha1Hash;
            std::string md5Hash;

            uint64_t txID;

        public:
            FileinfoEvent(Record* eventData);

            std::string getFileName() const { return fileName; }
            uint64_t getTxID() const { return txID; }

            std::string getSha256Hash() const { return sha256Hash; }
            std::string getSha1Hash() const { return sha1Hash; }
            std::string getMd5Hash() const { return md5Hash; }
    };
};