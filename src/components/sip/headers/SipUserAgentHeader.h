//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPUSERAGENTHEADER_H
#define CORE_SIPUSERAGENTHEADER_H


#include "SipHeader.h"

class SipUserAgentHeader : public SipHeader {

public:

    SipUserAgentHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "User-Agent";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::USER_AGENT;
    }

};


#endif //CORE_SIPUSERAGENTHEADER_H
