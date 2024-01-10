#include "NTLMSSP.hpp"

using namespace Capefox;

NTLMSSP::NTLMSSP(Record* data) {
    domain = data->valueAtAs<std::string>("domain");
    user = data->valueAtAs<std::string>("user");
    host = data->valueAtAs<std::string>("host");
    version = data->valueAtAs<std::string>("version");

    delete data;
}