#include <iostream>
#include <regex>
#include <string>
#include <filesystem>
#include <getopt.h>

#include "Suricata.hpp"

#include "ReportAnalyzer.hpp"
#include "ReportWriters/RSTReportWriter.hpp"

using namespace Capefox;

// Simple function printing the help menu whenever the user put the -h or --help command-line option
void printHelp() {
    std::cout << "CapeFox the suricata pcap analyzer" << std::endl << std::endl;
    std::cout << "\t--help/-h     : Display this help menu" << std::endl;
    std::cout << "\t--file/-f     : Path to the .pcap or eve.json to analize" << std::endl;
    std::cout << "\t--output/-o   : Report output path (If empty the report will be printed on screen)" << std::endl;
    std::cout << "\t--config/-c   : Path to a custom suricata config" << std::endl;
    std::cout << "\t--suricata/-s : Path to a custom suricata executable" << std::endl;

}

// The entry point of Capefox, parses command-line options with getopt setup the required input output
// streams and run the suricata ReportAnalazer.
// If a .pcap is passed as an input, then suricata will be ran to produce the required eve.json
int main(int argc, char** argv) {
    // Theses variables can be set by the use of the various command-line options and control the
    // behavior of capefox
    int fileFlag = false;
    std::string inputPath = "";
    std::string outputPath = "";
    std::string configPath = "config/suricata.yaml";
    std::string suricataPath = "suricata";

    // This structure defines the avalible command-line options
    struct option longOpt[] {
        {"help", no_argument, 0, 'h'},
        {"file", required_argument, &fileFlag, 'f'},
        {"output", required_argument, 0, 'o'},
        {"config", required_argument, 0, 'c'},
        {"suricata", required_argument, 0, 's'}
    };

    // This while loop parses the command-line options and fills in the resulting flags and variables
    int optIndex = 0;
    int optID = 0;
    while ((optID = getopt_long(argc, argv, "hf:o:c:s:", longOpt, &optIndex)) != -1) {
        switch (optID) {
            case 'h':
                printHelp();
                exit(0);
                break;

            case 'f':
                fileFlag = true;
                inputPath = optarg;
                break;
            
            case 'o':
                outputPath = optarg;
                break;

            case 'c':
                configPath = optarg;
                break;

            case 's':
                suricataPath = optarg;
                break;

            default:
                break;
        }
    }

    // Capefox cannot run without an input file, so if that happens then we print out an usage guide
    if (!fileFlag) {
        std::cout << "USAGE: " << argv[0] << " -f input/.json/.pcap [-o output.rst[OPTIONAL]]" << std::endl;
        return 0;
    }

    // If we get a pcap as an input then we first need to pass it through surricata
    if (std::regex_search(inputPath, std::regex("^.*\\.pcap$"))) {
        std::filesystem::create_directory("data");
        std::remove("data/eve.json");
        Suricata meerkat(suricataPath);
        if (meerkat.processPcap(inputPath, "data/", configPath, "rules/emerging-all.rules")) exit(2);
        inputPath = "data/eve.json";
    }

    // If we don't support the incomming file type then we exit.
    if (!std::regex_search(inputPath, std::regex("^.*\\.json$"))) {
        std::cout << "ERROR: Unknown file type" << std::endl;
        exit(1);
    }

    // Finally we load in suricata's eve file
    std::ifstream input(inputPath);
    // And load it in the report analyzer
    ReportAnalyzer analyzer(input, new RSTReportWriter());
    input.close();

    // When ready we write out the report either to the terminal or to a .rst report file
    if (outputPath == "") {
        analyzer.writeReport(std::cout);
    } else {
        std::ofstream output(outputPath);
        analyzer.writeReport(output);
        output.close();
    }

    return 0;
}