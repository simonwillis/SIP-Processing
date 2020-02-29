//
// Created by Simon Willis on 24/05/2016.
//

#ifndef TESTSIPPARSING_SIPUNKNOWNMESSAGE_H
#define TESTSIPPARSING_SIPUNKNOWNMESSAGE_H


#include "SipMessage.h"
#include "SipRequest.h"

class SipUnknownRequest : public SipRequest {

public:

    SipUnknownRequest(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers)
            : SipRequest(propertiesLine, headers) {

    }

    virtual const SipMessageType getRequestType() override {
        return SipMessageType::UNKNOWN;
    }

    virtual const std::string getMessageContent() override {
        return std::string();
    }

    virtual const char *getRequestMethod() override {
        return "UNKNOWN";
    }

    virtual const bool supported() override {
        return false;
    }

};


#endif //TESTSIPPARSING_SIPUNKNOWNMESSAGE_H
