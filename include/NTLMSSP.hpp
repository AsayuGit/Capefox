#pragma once

#include "Record.hpp"

namespace Capefox {
    class NTLMSSP {
        private:
            std::string domain;
            std::string user;
            std::string host;
            std::string version;

        public:
            NTLMSSP(Record* data);

            std::string getDomain() const { return domain; }
            std::string getUser() const { return user; }
            std::string getHost() const { return host; }
            std::string getVersion() const { return version; }
    };
};