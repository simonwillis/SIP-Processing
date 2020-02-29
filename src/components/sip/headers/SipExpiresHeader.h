//
// Created by Simon Willis on 25/05/2016.
//

#ifndef TESTSIPPARSING_SIPEXPIRESHEADER_H
#define TESTSIPPARSING_SIPEXPIRESHEADER_H


#include "SipHeader.h"

class SipExpiresHeader : public SipHeader {

public:

    SipExpiresHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Expires";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::EXPIRES;
    }

};


#endif //TESTSIPPARSING_SIPEXPIRESHEADER_H
