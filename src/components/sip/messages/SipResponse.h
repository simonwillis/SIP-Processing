//
// Created by Simon Willis on 31/05/2016.
//

#ifndef SIP_MESSAGES_SIPRESPONSE_H
#define SIP_MESSAGES_SIPRESPONSE_H


#include "SipMessage.h"

#include <sstream>

#define DEBUG_SIPRESPONSE true

class SipResponse : public SipMessage {

public:

    SipResponse(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers, std::map<SipMessageBody::BodyType, std::string> bodies = std::map<SipMessageBody::BodyType, std::string>())
            : SipMessage(propertiesLine, headers, bodies) {

    }

    virtual const SipMessageType getResponseType() = 0;

    const char * getTypeDescriptionString() override {
        return "Response";
    }

    const char * getSubTypeDescriptionString() override {
        return getResponseDescription();
    }

    virtual const uint32_t getResponseCode() = 0;

    virtual const char *getResponseDescription() = 0;

    virtual const bool supported() override {
        return false;
    }

    virtual const bool isRequest() override {
        return false;
    }

    virtual const bool isResponse() override {
        return true;
    }

    virtual const std::string getMessageBody() override {
        std::stringstream ss;

        //TODO Handle content where contentLength in Response is not 0

        //TODO Handle extra header properties in Reponses where required

        ss << "SIP/2.0/" << SipMessage::getTransportTypeStr(transportType);
        ss << ' ';
        ss << getResponseCode();
        ss << ' ';
        ss << getResponseDescription();
        ss << "\r\n";
        ss << "Via: " << getResponseViaContent();
        ss << "\r\n";
        ss << "From: " << from;
        ss << "\r\n";
        ss << "To: " << to << ";tag=" << localTag;
        ss << "\r\n";
        ss << "Call-ID: " << callId;
        ss << "\r\n";
        ss << "CSeq: " << cseq;
        ss << "\r\n";
        ss << "User-Agent: " << UserAgentId;
        ss << "\r\n";
        ss << "Content-Length: " << getContentLength();
        ss << "\r\n";
        ss << "\r\n";

        return ss.str();
    }

private:
    std::string via;
    std::string from;
    std::string to;
    std::string callId;
    std::string cseq;
    std::string localTag;

};


#endif //SIP_MESSAGES_SIPRESPONSE_H
