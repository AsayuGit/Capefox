#include "Events/SmbEvent.hpp"

using namespace Capefox;

SmbEvent::SmbEvent(Record* eventData) : IPEvent(eventData) {
    eventType = EventType::Smb;

    Record* smbData = data->valueAt("smb");

    command = smbData->valueAtAs<std::string>("command");
    if (command == "SMB1_COMMAND_SESSION_SETUP_ANDX" ) {
        Record* smbResponse = smbData->valueAt("response");
        clientOS = smbResponse->valueAtAs<std::string>("native_lm");
        delete smbResponse;
    }

    if (Record* ntlmsspData = smbData->valueAt("ntlmssp")) {
        ntlmssp = new NTLMSSP(ntlmsspData);

        if (clientOS != "") clientOS += " - ";
        clientOS += ntlmssp->getVersion();
    }

    delete smbData;
}

SmbEvent::~SmbEvent() {
    if (ntlmssp) {
        delete ntlmssp;
    }
}