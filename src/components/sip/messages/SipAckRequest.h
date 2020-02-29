//
// Created by Simon Willis on 24/05/2016.
//

#ifndef SIP_MESSAGES_SIPACKMESSAGE_H
#define SIP_MESSAGES_SIPACKMESSAGE_H


#include "SipMessage.h"
#include "SipRequest.h"

class SipAckRequest : public SipRequest {

public:

    SipAckRequest(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers)
            : SipRequest(propertiesLine, headers) {

    }

    virtual const SipMessageType getRequestType() override {
        return SipMessageType::ACK;
    }

    virtual const char *getRequestMethod() override {
        return "ACK";
    }

    virtual const bool supported() override {
        return false;
    }

};


#endif //SIP_MESSAGES_SIPACKMESSAGE_H
