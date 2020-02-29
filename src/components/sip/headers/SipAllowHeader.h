//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPALLOWEVENTSHEADER_H
#define CORE_SIPALLOWEVENTSHEADER_H


#include "SipHeader.h"

class SipAllowHeader : public SipHeader {

public:

    SipAllowHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Allow";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::ALLOW;
    }

};


#endif //CORE_SIPALLOWEVENTSHEADER_H
