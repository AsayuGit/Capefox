#include "Suricata.hpp"

#include <cstdlib>

using namespace Capefox;

// This contructor allow the customization the suricata's path
Suricata::Suricata(std::string path) {
    suricataPath = path;
}

// This methods calls surricata to process the .pcap file and generate a .eve file for futher analysis
int Suricata::processPcap(const std::string inputPath, const std::string outputDir, const std::string configPath, const std::string rulesPath) {
    // We first prepare the command with the specified suricata executable, config, input, output dir and ruleset
    std::string cmd = suricataPath + " -c " + configPath + " -r " + inputPath + " -l " + outputDir + " -S " + rulesPath;

    // Then we execute it
    return std::system(cmd.c_str());
}