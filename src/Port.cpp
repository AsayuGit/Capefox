#include "Port.hpp"

using namespace Capefox;

Port::Port(uint16_t portNB) {
    this->portNB = portNB;
}

// This methods infers the service type from a list of known service ports
std::string Port::getService() const {
    // This list is not exhaustive and should be extended
    switch (portNB) {
        case 135:
            return "RPC";

        case 389:
            return "LDAP";

        default:
            // If the relevant service is not in the list then we itentify it as
            // Unknows with the port number appended for human analysis
            return "Unknown (" + std::to_string(portNB) + ")";
    }
}