//
// Created by Simon Willis on 31/05/2016.
//

#ifndef SIP_MESSAGES_SIPTRYINGMESSAGE_H
#define SIP_MESSAGES_SIPTRYINGMESSAGE_H


#include "SipMessage.h"
#include "SipResponse.h"

class SipTryingResponse : public SipResponse {

public:

    SipTryingResponse(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers)
            : SipResponse(propertiesLine, headers) {
    }

    virtual const SipMessageType getResponseType() override {
        return SipMessageType::TRYING;
    }

    virtual const bool supported() override {
        return true;
    }

    virtual const uint32_t getResponseCode() override {
        return 100;
    }

    virtual const char *getResponseDescription() override {
        return "Trying";
    }

};


#endif //SIP_MESSAGES_SIPTRYINGMESSAGE_H
