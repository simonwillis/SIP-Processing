//
// Created by Simon Willis on 25/05/2016.
//

#ifndef TESTSIPPARSING_SIPMIMEVERSIONHEADER_H
#define TESTSIPPARSING_SIPMIMEVERSIONHEADER_H


#include "SipHeader.h"

class SipMimeVersionHeader : public SipHeader {

public:

    SipMimeVersionHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Mime-Version";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::MIME_VERSION;
    }

};


#endif //TESTSIPPARSING_SIPMIMEVERSIONHEADER_H
