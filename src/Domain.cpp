#include "Domain.hpp"

#include <regex>

using namespace Capefox;

// The domain class more than just holding the domain name, can infer the domain type
Domain::Domain(std::string name) {
    this->name = name;

    // Using a regex we know that any domain containing __ldap._tcp is part of the AD
    if (std::regex_search(name, std::regex("_ldap._tcp"))) {
        // That if it then contains pcd._msdcs it is the primary domain controller
        if (std::regex_search(name, std::regex("pdc._msdcs"))) {
            this->type = Domain::ADDomainPDC;
        // Else that if it contains dc._msdcs it is one of the domain controllers
        } else if (std::regex_search(name, std::regex("dc._msdcs"))) {
            this->type = Domain::ADDomainDC;
        } else {
            this->type = Domain::ADDomain;
        }
    }
}