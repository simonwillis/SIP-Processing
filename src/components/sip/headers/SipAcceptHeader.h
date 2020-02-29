//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPACCEPTHEADER_H
#define CORE_SIPACCEPTHEADER_H


#include "SipHeader.h"

class SipAcceptHeader : public SipHeader {

public:

    SipAcceptHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Accept";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::ACCEPT;
    }

};


#endif //CORE_SIPACCEPTHEADER_H
