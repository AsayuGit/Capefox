#pragma once

#include <string>

namespace Capefox {
    // Interfaces with the local suricata install to build eve.json from .pcap s
    class Suricata {
        private:
            std::string suricataPath = "suricata";

        public:
            Suricata() = default;
            Suricata(std::string path);

            // This method interfaces with suricata with the privided arguments
            int processPcap(const std::string inputPath, const std::string outputDir, const std::string configPath, const std::string rulesPath);
    };
};