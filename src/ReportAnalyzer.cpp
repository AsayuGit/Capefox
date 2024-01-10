#include "ReportAnalyzer.hpp"

#include <algorithm>
#include <ranges>

#include "JSONRecord.hpp"

#include "Event.hpp"
#include "Events/AlertEvent.hpp"
#include "Events/DnsEvent.hpp"
#include "Events/FileinfoEvent.hpp"
#include "Events/FlowEvent.hpp"
#include "Events/SmbEvent.hpp"
#include "Events/Krb5Event.hpp"

using namespace Capefox;

ReportAnalyzer::ReportAnalyzer(std::ifstream& input, ReportWriter* writer) {
    this->writer = writer;
    
    // Loads events from input
    std::string line;
    while (std::getline(input, line)) {
        Event* newEvent = Event::New(new JSONRecord(line));
        events.push_back(newEvent);
    }
}

ReportAnalyzer::~ReportAnalyzer() {
    for (Event* event : events) delete event;

    delete writer;
}

// This is the main method of Report analyzer, from which all the rest are run.
void ReportAnalyzer::writeReport(std::ostream& output) {
    output << writer->WriteMainTitle("Capefox Report");

    output << writer->WriteTitle("EVE Stats :");
    output << "| " << writer->Bold("Start:") << events.front()->getTimeStamp() << std::endl;
    output << "| " << writer->Bold("End  :") << events.back()->getTimeStamp() << std::endl << std::endl;

    // The the various tests composing the outgoing report file.
    printPrivateAddress(output);
    printWindowsDomains(output);
    printADUsers(output);
    printSMBClientOSVersion(output);
    printTCPIPServices(output);

    printSignatures(output);
    printMalwareList(output);
    printImpactedHosts(output);
    printFileHashes(output);
}

// Simple convinience method to the list of events of a particuliar type
std::vector<Event*> ReportAnalyzer::getEventType(EventType type) const {
    std::vector<Event*> eventList;
    // Copy events from the main eventlist if their event type match the required type
    std::ranges::copy_if(events, std::back_inserter(eventList), [type](const Event* event) {
        return (event->getEventType() == type);
    });

    return eventList;
}

// This method fetch the IP assotiated to an hostname (if any) from the dns records
// and returns it.
IP ReportAnalyzer::getIPForHostname(std::string hostname) const {
    IP address;

    // We first get the list of all DNS Answer events
    std::vector<Event*> dnsEvents;
    std::ranges::copy_if(events, std::back_inserter(dnsEvents), [](const Event* event){
        return (event->getEventType() == EventType::Dns) && (((DnsEvent*)event)->getRequestType() == DnsEvent::DNSRequestType::Answer);
    });

    // Then for each of them
    for (Event* event : dnsEvents) {
        DnsEvent* dnsEvent = static_cast<DnsEvent*>(event);

        const std::vector<DnsRecord*>& dnsRecords = dnsEvent->getAnswerRecords();
        auto lbd = [hostname](const DnsRecord* record){
            return std::string(record->getDomain()) == hostname;
        };
        // If their hostname match
        auto record = std::find_if(dnsRecords.begin(), dnsRecords.end(), lbd);
        if (record != dnsRecords.end()) {
            // Then we get the assotiated ip address and break out the loop
            address = (*record)->getAddress();
            break;
        }
    }

    return address;
}

// This method fetch the hostname assotiated to and IP (if any) from the dns records
// and returns it.
std::string ReportAnalyzer::getHostnameForIP(IP address) const {
    std::string hostname;

    // First we get a list of all DNS Answer events
    std::vector<Event*> dnsEvents;
    std::ranges::copy_if(events, std::back_inserter(dnsEvents), [](const Event* event){
        return (event->getEventType() == EventType::Dns) && (((DnsEvent*)event)->getRequestType() == DnsEvent::DNSRequestType::Answer);
    });

    // Then for each DNS event
    for (Event* event : dnsEvents) {
        DnsEvent* dnsEvent = static_cast<DnsEvent*>(event);

        const std::vector<DnsRecord*>& dnsRecords = dnsEvent->getAnswerRecords();
        auto lbd = [address](const DnsRecord* record){
            return record->getAddress() == address;
        };
        
        // We check if their IP address match
        auto record = std::find_if(dnsRecords.begin(), dnsRecords.end(), lbd);
        if (record != dnsRecords.end()) {
            // and if so then we get the assotiated hostname and break out the loop.
            hostname = (*record)->getDomain();
            break;
        }
    }

    return hostname;
}

// This method looks for and print out all private ip addresses present.
void ReportAnalyzer::printPrivateAddress(std::ostream& output) {
    output << writer->WriteTitle("Private Addresses :");

    // So first we get a list of all events with an IP address, so everything that is not Unknown or a Stat event.
    std::vector<Event*> ipEvents;
    std::ranges::copy_if(events, std::back_inserter(ipEvents), [](const Event* event){ return (event->getEventType() != EventType::Unknown) && (event->getEventType() != EventType::Stat); });

    // Then we construct a list of private IP addresses
    std::vector<IP> prvIP;
    // To to so, for each event with an IP address
    for (Event* event : ipEvents) {
        IPEvent* ipEvent = static_cast<IPEvent*>(event);
        
        // We check if one of the IP is private by the use of their isPrivate() method
        if (ipEvent->getDstIp().isPrivate()) {
            // If they are private then we append them to the list of private IP addresses
            prvIP.push_back(ipEvent->getDstIp());
        }

        // We do the same with the source addresses
        if (ipEvent->getSrcIp().isPrivate()) {
            prvIP.push_back(ipEvent->getSrcIp());
        }
    }

    // Then in order to remove any duplicates, we first sort the list from smallest to biggest
    std::ranges::sort(prvIP, [](const IP a, const IP b) { return a < b; });
    // And then remove any contiguous duplicates.
    std::vector<IP> uniqPrvIP;
    std::ranges::unique_copy(prvIP, std::back_inserter(uniqPrvIP),[](const IP a, const IP b) { return a == b; });

    // Finally, for each private IP in the list we print it to the screen as a bullet point list.
    for (IP ip : uniqPrvIP) {
        output << "- " << ip << std::endl;
    } output << std::endl;
}

// This method looks for any windows active directory domains and print them out
void ReportAnalyzer::printWindowsDomains(std::ostream& output) {
    // To do so we first get a list of all DNS events meant for the LAN (NXDOMAIN) and that expose a service (SRV)
    std::vector<Event*> dnsEvents;
    std::ranges::copy_if(events, std::back_inserter(dnsEvents), [](const Event* event){
        return (event->getEventType() == EventType::Dns) &&
               ((((DnsEvent*)event)->getRCode() == "NXDOMAIN") || (((DnsEvent*)event)->getRRType() == "SRV")) &&
               ((((DnsEvent*)event)->getSrcIp().isPrivate()) || (((DnsEvent*)event)->getDstIp().isPrivate()));
    });

    // Then in order to remove duplicate we first sort the resulting DNS events alphabetically
    std::ranges::sort(dnsEvents, [](const Event* a, const Event* b){
        return std::string(((DnsEvent*)a)->getRRName()) < std::string(((DnsEvent*)b)->getRRName());
    });

    // And remove any consecutive duplicates
    std::vector<Event*> uniqDnsEvents;
    std::ranges::unique_copy(dnsEvents, std::back_inserter(uniqDnsEvents),[](const Event* a, const Event* b) {
        return ((DnsEvent*)a)->getRRName() == ((DnsEvent*)b)->getRRName();
    });

    // Then for each category of AD domain we print them out on the scree,
    output << writer->WriteTitle("Windows Domains :");
    for (Event* event : uniqDnsEvents) {
        DnsEvent* dnsEvent = static_cast<DnsEvent*>(event);

        // The Domain's class getType() method is responsible for identifying
        // a domain name as part of the active directory
        if (dnsEvent->getRRName().getType() == Domain::ADDomain) {
            output << "- " << dnsEvent->getRRName() << std::endl;
        }
    } output << std::endl;

    // We do the same for the domain controllers
    output << writer->WriteTitle("Windows Domain Controllers :");
    for (Event* event : uniqDnsEvents) {
        DnsEvent* dnsEvent = static_cast<DnsEvent*>(event);

        if (dnsEvent->getRRName().getType() == Domain::ADDomainDC) {
            output << "- " << dnsEvent->getRRName() << std::endl;
        }
    } output << std::endl;

    // And for the primary domain controller
    output << writer->WriteTitle("Windows Primary Domain Controller :");
    for (Event* event : uniqDnsEvents) {
        DnsEvent* dnsEvent = static_cast<DnsEvent*>(event);

        if (dnsEvent->getRRName().getType() == Domain::ADDomainPDC) {
            output << "- " << dnsEvent->getRRName() << std::endl;
        }
    } output << std::endl;

}

// This methods looks for and print out any active directory username from the smb and kerberos events
void ReportAnalyzer::printADUsers(std::ostream& output) {
    output << writer->WriteTitle("Active Directory users :");

    // We first handle the SMB users
    output << writer->WriteSubTitle("SMB users :");

    // We first get a list of the relevant SMB events. We're particularly intreasted in the session setup since
    // that is when usernames are going to be exchanged.
    std::vector<Event*> smbEvents;
    std::ranges::copy_if(events, std::back_inserter(smbEvents), [](const Event* event) {
        return (event->getEventType() == EventType::Smb) &&
        // To do so we match on two smb commands : "SMB1_COMMAND_SESSION_SETUP_ANDX" for SMB1 sessions
        // and "SMB2_COMMAND_SESSION_SETUP" for SMB2 sessions
        ((((SmbEvent*)event)->getCommand() == "SMB1_COMMAND_SESSION_SETUP_ANDX") || (((SmbEvent*)event)->getCommand() == "SMB2_COMMAND_SESSION_SETUP")) &&
        // We obviously don't wand any event not containing usernames so we filter them out
        (((SmbEvent*)event)->getNTLMSSP() != nullptr) && (((SmbEvent*)event)->getNTLMSSP()->getUser() != "");
    });

    // As always to remove any duplicates we first sort them alphabetically
    std::ranges::sort(smbEvents, [](const Event* a, const Event* b) { return ((SmbEvent*)a)->getNTLMSSP()->getUser() < ((SmbEvent*)b)->getNTLMSSP()->getUser(); });
    
    // And remove consecutives duplicates
    std::vector<Event*> uniqSmbEvents;
    std::ranges::unique_copy(smbEvents, std::back_inserter(uniqSmbEvents),[](const Event* a, const Event* b) {
        return ((SmbEvent*)a)->getNTLMSSP()->getUser() == ((SmbEvent*)b)->getNTLMSSP()->getUser();
    });

    // Then for each unique smb event
    for (Event* event : uniqSmbEvents) {
        SmbEvent* smbEvent = static_cast<SmbEvent*>(event);

        // We print the user name in the report as a bulletpoint list
        output << "- " << smbEvent->getNTLMSSP()->getUser() << std::endl;
    } output << std::endl;

    // Then we do the same treatement for the kerberos users
    output << writer->WriteSubTitle("Kerberos users :");

    // By first fetch the list of KRB_TGS_REP kerberos events.
    std::vector<Event*> krbEvents;
    std::ranges::copy_if(events, std::back_inserter(krbEvents), [](const Event* event) {
        return (event->getEventType() == EventType::Krb5) &&
        // Unfortunately this list doesn't contain only username so we need to filter out any machine hostname
        // by removing any cname ending with a '$'
        (((Krb5Event*)event)->getMessageType() == "KRB_TGS_REP") && (((Krb5Event*)event)->getCName().back() != '$');
    });

    // Then we sort and remove duplicated cnames
    std::ranges::sort(krbEvents, [](const Event* a, const Event* b) { return ((Krb5Event*)a)->getCName() < ((Krb5Event*)b)->getCName(); });
    std::vector<Event*> uniqKrbEvents;
    std::ranges::unique_copy(krbEvents, std::back_inserter(uniqKrbEvents),[](const Event* a, const Event* b) {
        return ((Krb5Event*)a)->getCName() == ((Krb5Event*)b)->getCName();
    });

    // And finally for each resulting kerberos events
    for (Event* event : uniqKrbEvents) {
        Krb5Event* krbEvent = static_cast<Krb5Event*>(event);

        // We print the username in the report as a bullerpoint list
        output << "- " << krbEvent->getCName() << std::endl;
    } output << std::endl;
}

// This method print the SMB client os version by looking at smb events
void ReportAnalyzer::printSMBClientOSVersion(std::ostream& output) {
    output << writer->WriteTitle("SMB Client OSs :");

    // So first, we get a list of smb events witch contains any client os information
    std::vector<Event*> smbEvents = getEventType(EventType::Smb);
    std::ranges::copy_if(events, std::back_inserter(smbEvents), [](const Event* event) {
        return (event->getEventType() == EventType::Smb) && (((SmbEvent*)event)->getClientOS() != "");
    });

    // Then for each resulting event
    for (Event* event : smbEvents) {
        SmbEvent* smbEvent = static_cast<SmbEvent*>(event);

        // We print the client os versions in the record as a bulletpoint list
        // SMB's getClientOS() method is responsible for correctly fetching this information.
        std::string clientOS = smbEvent->getClientOS();
        if (clientOS != "") {
            output << "- " << clientOS << std::endl;
        }
    }
}

// This method finds and print any TCP/IP service offered in the LAN
void ReportAnalyzer::printTCPIPServices(std::ostream& output) {
    output << writer->WriteTitle("Local TCP/IP Services :");

    // First we need a list of Flow event containing TCP
    std::vector<Event*> flowEvents;
    std::ranges::copy_if(events, std::back_inserter(flowEvents), [](const Event* event) {
        return (event->getEventType() == EventType::Flow) && ((FlowEvent*)event)->getProtocol() == "TCP";
    });

    // Then we attempt to create the list of the contained services
    std::vector<std::string> services;
    for (Event* event : flowEvents) {
        FlowEvent* flowEvent = static_cast<FlowEvent*>(event);

        // We can simply fetch the app protocol (if any) from the flow event.
        std::string appProtocol = flowEvent->getAppProtocol();
        
        // But then we try to extract more information from the port
        // To do so we need the server side port and the the client side port
        Port port;
        // Since we're looking for services on the LAN we check that their IP is private
        // If both are then the server port is likely to be the smalles so we pick
        // the min of the sr_cport and the dst_port
        if (flowEvent->getSrcIp().isPrivate() && flowEvent->getDstIp().isPrivate()) {
            port = std::min(flowEvent->getSrcPort(), flowEvent->getDstPort());
        } else if (flowEvent->getSrcIp().isPrivate()) { // If they're not both private then we pick the
            port = flowEvent->getSrcPort();             // port of the private IP address, src or dest
        } else if (flowEvent->getDstIp().isPrivate()) {
            port = flowEvent->getDstPort();
        } else { // If neither of them are private then they're not what we're looking for
            continue; // and we can just continue to the next event.
        }

        // If the port is bigger than 32767 (So half the range) then its unlikely
        // to be a server port and more likey client machines answering the internet
        if (port > (UINT16_MAX / 2)) continue; // So we just continue to the next event

        // Finaly we choose the best source of information.
        if ((appProtocol == "") || (appProtocol == "failed")) {
            // If the app protocol is invalid then we try to infer it 
            // for the server port. The Port's class getService() method
            // is responsible for guessing the service type from the port number
            services.push_back(port.getService());
        } else {
            // If the app protocol is valid then we use it 
            services.push_back(appProtocol);
        }
    }

    // Finally we sort our services list alphabetically
    std::ranges::sort(services, [](const std::string a, const std::string b) { return a < b; });
    // And remove any duplicated services
    std::vector<std::string> uniqServices;
    std::ranges::unique_copy(services, std::back_inserter(uniqServices), [](const std::string a, const std::string b) { return a == b; });
    // Then print them out to the report as a bulletpoint list.
    for (std::string service : uniqServices) {
        output << "- " << service << std::endl;
    } output << std::endl;
}

// This method print then triggered signatures
void ReportAnalyzer::printSignatures(std::ostream& output) {
    output << writer->WriteTitle("Signatures :");

    // To do so we fist get a list of every alerts event
    std::vector<Event*> alertEvent = getEventType(EventType::Alert);
    // And remove duplicates
    std::vector<Event*> uniqAlertEvent;
    std::ranges::unique_copy(alertEvent, std::back_inserter(uniqAlertEvent), [](const Event* eventA, const Event* eventB) {
        return (((AlertEvent*)eventA)->getSignatureID() == ((AlertEvent*)eventB)->getSignatureID());
    });

    // Then for each alert
    for (Event* event : uniqAlertEvent) {
        AlertEvent* alertEvent = static_cast<AlertEvent*>(event);

        // We print the signature id, signature and category to the report.
        // We make sure to escape the signature itself before writing as its format can break
        // RST formating
        output << "- [" << alertEvent->getSignatureID() << "] " << writer->Escape(alertEvent->getSignature()) << " --- " << alertEvent->getCategory() << std::endl;
    } output << std::endl;
}

// This method prints the list of detected malwares
void ReportAnalyzer::printMalwareList(std::ostream& output) {
    output << writer->WriteTitle("Detected Malwares :");

    // To do so we first need to get a list of all alerts
    std::vector<Event*> alertEvent = getEventType(EventType::Alert);
    // And remove any duplicate
    std::vector<Event*> uniqAlertEvent;
    std::ranges::unique_copy(alertEvent, std::back_inserter(uniqAlertEvent), [](const Event* eventA, const Event* eventB) {
        return (((AlertEvent*)eventA)->getSignatureID() == ((AlertEvent*)eventB)->getSignatureID());
    });

    // Then for each unique alert
    for (Event* event : uniqAlertEvent) {
        AlertEvent* alertEvent = static_cast<AlertEvent*>(event);

        // We get the malware (family if any)
        std::string malware =  alertEvent->getMetadata("malware_family");
        if (malware != "") {
            // And print them out to the report as a bulletpoint list
            output << "- " << malware << std::endl;
        }
    } output << std::endl;
}

// This method prints the list of impacted hosts
void ReportAnalyzer::printImpactedHosts(std::ostream& output) {
    output << writer->WriteTitle("Targeted Hosts :");

    // We first need to get a list of all alert events that have a target
    std::vector<Event*> alertEvent;
    std::ranges::copy_if(events, std::back_inserter(alertEvent), [](const Event* event) {
        return (event->getEventType() == EventType::Alert) && ((AlertEvent*)event)->hasTarget();
    });
    // Then as usual sort
    std::ranges::sort(alertEvent, [](const Event* a, const Event* b){
        return ((AlertEvent*)a)->getTargetAddress() == ((AlertEvent*)b)->getTargetAddress();
    });
    // And remove any duplicates
    std::vector<Event*> uniqAlertEvent;
    std::ranges::unique_copy(alertEvent, std::back_inserter(uniqAlertEvent), [](const Event* eventA, const Event* eventB) {
        return (((AlertEvent*)eventA)->getTargetAddress() == ((AlertEvent*)eventB)->getTargetAddress());
    });

    // Then for each unique alert event
    for (Event* event : uniqAlertEvent) {
        AlertEvent* alertEvent = static_cast<AlertEvent*>(event);

        // We get the host target of the attack
        IP host = alertEvent->getTargetAddress();
        
        // And print it to the report
        output << "- " << host;
        
        // If we're able to locate its hostname
        std::string hostname = getHostnameForIP(host);
        if (hostname != "") {
            // then we print it alongside
            output << " --- " << hostname;
        } output << std::endl;

    } output << std::endl;
}

// This method print the sha256 hash of ever file present in the input that has triggered an alert
void ReportAnalyzer::printFileHashes(std::ostream& output) {
    output << writer->WriteTitle("FileHash :");

    // In oder to get the file hashes we first need to get the list of every Fileinfo event
    std::vector<Event*> fileEvents = getEventType(EventType::Fileinfo);

    // Then we sort every hash alphabetically
    std::ranges::sort(fileEvents, [](const Event* a, const Event* b) {
        return ((FileinfoEvent*)a)->getSha256Hash() < ((FileinfoEvent*)b)->getSha256Hash(); 
    });

    /// Then remove any duplicate (in case a file is transmitted twice)
    std::vector<Event*> uniqFileEvents;
    std::ranges::unique_copy(fileEvents, std::back_inserter(uniqFileEvents), [](const Event* eventA, const Event* eventB) {
        return (((FileinfoEvent*)eventA)->getSha256Hash() == ((FileinfoEvent*)eventB)->getSha256Hash());
    });

    // Then to cross reference them with any triggered alert we get the list of all alert events
    std::vector<Event*> alertEvents = getEventType(EventType::Alert);

    // And for each unique file
    for (Event* event : uniqFileEvents) {
        FileinfoEvent* file = static_cast<FileinfoEvent*>(event);

        // Check to see if its flow id match a triggered signature
        auto lbd = [file](const Event* alert){return (static_cast<const AlertEvent*>(alert)->getFlowID() == file->getFlowID());};
        std::vector<Event*>::iterator fileAlert = std::find_if(alertEvents.begin(), alertEvents.end(), lbd);
        if (fileAlert != alertEvents.end()) {
            // And if one matches then we print the file hash
            output << "- [" << file->getFlowID() << "] " << file->getSha256Hash() << std::endl;
            output << std::endl;
        }
        // Followed by any alert that matched
        for (; fileAlert != alertEvents.end(); fileAlert = std::find_if(++fileAlert, alertEvents.end(), lbd)) {
            output << "\t- " << writer->Bold(static_cast<AlertEvent*>(*fileAlert)->getCategory()) << std::endl;
        }
    } output << std::endl;
}
