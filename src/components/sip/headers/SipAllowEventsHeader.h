//
// Created by Simon Willis on 25/05/2016.
//

#ifndef TESTSIPPARSING_SIPALLOWEVENTSHEADER_H
#define TESTSIPPARSING_SIPALLOWEVENTSHEADER_H


#include "SipHeader.h"

class SipAllowEventsHeader : public SipHeader {

public:

    SipAllowEventsHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Allow-Events";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::AllOW_EVENTS;
    }

};


#endif //TESTSIPPARSING_SIPALLOWEVENTSHEADER_H
