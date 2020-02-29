//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPVIAHEADER_H
#define CORE_SIPVIAHEADER_H

#include <stdlib.h>
#include <sstream>

#include "SipHeader.h"

class SipViaHeader : public SipHeader {

public:
    SipViaHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

        // Example VIA Header Value: SIP/2.0/TCP client.example.com:5060;branch=z9hG4bKnashds7

        std::vector<mapped_buffer_t> sections = buffer_split(this->headerValue, ' ');
        if (sections.size() != 2) {
            fprintf(stderr, "SipViaHeader Cannot use content [%s], expected two sections separated by space, found %ld\n", this->headerValue.toString().c_str(), sections.size());
            //TODO handle failure to read data in header instantiation
            return;
        }

        // The first section of the VIA line holds (protocol)/(version)/(transport) standard two options are SIP/2.0/TCP or SIP/2.0/UDP
        if (buffer_ends_with(sections[0], "UDP")) {
            transportType = SipMessage::SipMessageTransportType::UDP;
        } else if (buffer_ends_with(sections[0], "TCP")) {
            transportType = SipMessage::SipMessageTransportType::TCP;
        } else {
            fprintf(stderr, "SipViaHeader Unknown connection type in VIA [%s]\n", sections[0].toString().c_str());
            transportType = SipMessage::DefaultTransportType;
        }

        // The second section of the VIA line holds (address):(port);branch=(branch)
        // Then it can have all sorts of stuff on the end of that, semicolon separated

        std::vector<mapped_buffer_t> subSections = buffer_split(sections[1], ';');

        for (size_t sectionIndex = 0; sectionIndex < subSections.size(); ++sectionIndex) {
            if (sectionIndex == 0) {
                std::vector<mapped_buffer_t> addressSections = buffer_split(subSections[0], ':');
                remoteAddress = addressSections[0];
                remotePort = (addressSections.size() > 1) ? atoi(addressSections[1].toString().c_str()) : 0;
            } else {
                // expecting name=value pairs here
                std::vector<mapped_buffer_t> kvp = buffer_split(subSections[sectionIndex], '=');
                if (kvp.size() == 2) {
                    if (buffer_equals(kvp[0], "branch")) {
                        remoteBranch = kvp[1];
                    } else if (buffer_equals(kvp[0], "rport")) {
                        if (! remotePort) {
                            remotePort = toLongInteger(kvp[1]);
                        }
                    }
                }
            }
        }
    }

    virtual const char *getFieldName() override {
        return "Via";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::VIA;
    }

    std::string getOriginalValue() {
        return headerValue.toString();
    }

    std::string getResponseValue() {
        std::stringstream ss;

//        ss << getOriginalValue();

        ss << "SIP/2.0/";
        ss << SipMessage::getTransportTypeStr(transportType);
        ss << " ";
        ss << getRemoteAddress();
        if (remoteBranch.len) {
            ss << ";branch=";
            ss << getRemoteBranch();
        }
        if (remoteAddress.len) {
            ss << ";received=";
            ss << getRemoteAddress();
        }
        if (remotePort) {
            ss << ";rport=";
            ss << remotePort;
        } else {
            ss << ";rport";
        }

        return ss.str();
    }

    SipMessage::SipMessageTransportType getTransportType() {
        return transportType;
    }

    std::string getRemoteAddress() {
        return remoteAddress.toString();
    }

    int getRemotePort() {
        return remotePort;
    }

    std::string getRemoteBranch() {
        return remoteBranch.toString();
    }


private:
    SipMessage::SipMessageTransportType transportType;
    mapped_buffer_t remoteAddress;
    mapped_buffer_t remoteBranch;
    int remotePort;

};


#endif //CORE_SIPVIAHEADER_H
