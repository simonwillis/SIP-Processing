//
// Created by Simon Willis on 24/05/2016.
//

#ifndef SIP_MESSAGES_SIPBYEMESSAGE_H
#define SIP_MESSAGES_SIPBYEMESSAGE_H


#include "SipMessage.h"
#include "SipRequest.h"

class SipByeRequest : public SipRequest {

public:

    SipByeRequest(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers)
            : SipRequest(propertiesLine, headers) {

    }

    virtual const SipMessageType getRequestType() override {
        return SipMessageType::BYE;
    }

    virtual const char *getRequestMethod() override {
        return "BYE";
    }

    virtual const bool supported() override {
        return false;
    }

};


#endif //SIP_MESSAGES_SIPBYEMESSAGE_H
