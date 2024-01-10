#pragma once

#include <fstream>
#include <vector>

#include "ReportWriter.hpp"
#include "EventType.hpp"

namespace Capefox {
    class IP;
    class Event;
    class ReportAnalyzer {
        private:
            std::vector<Event*> events;
            ReportWriter* writer;

        public:
            ReportAnalyzer(std::ifstream& input, ReportWriter* writer);
            ~ReportAnalyzer();

            void writeReport(std::ostream& output);

        private:
            std::vector<Event*> getEventType(EventType type) const;
            IP getIPForHostname(std::string hostname) const;
            std::string getHostnameForIP(IP address) const;

            void printPrivateAddress(std::ostream& output);
            void printWindowsDomains(std::ostream& output);
            void printADUsers(std::ostream& output);
            void printSMBClientOSVersion(std::ostream& output);
            void printTCPIPServices(std::ostream& output);

            void printSignatures(std::ostream& output);
            void printMalwareList(std::ostream& output);
            void printImpactedHosts(std::ostream& output);
            void printFileHashes(std::ostream& output);
    };
};