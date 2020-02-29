//
// Created by Simon Willis on 26/05/2016.
//

#ifndef TESTSIPPARSING_SIPOPTIONSMESSAGE_H
#define TESTSIPPARSING_SIPOPTIONSMESSAGE_H


#include "SipMessage.h"
#include "SipRequest.h"

class SipOptionsRequest : public SipRequest {

    //TODO Add implementation details for SipOptionsRequest

public:

    SipOptionsRequest(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers)
            : SipRequest(propertiesLine, headers) {

    }

    virtual const SipMessageType getRequestType() override {
        return SipMessageType::OPTIONS;
    }

    virtual const char *getRequestMethod() override {
        return "OPTIONS";
    }

    virtual const bool supported() override {
        return 0;
    }


};


#endif //TESTSIPPARSING_SIPOPTIONSMESSAGE_H
