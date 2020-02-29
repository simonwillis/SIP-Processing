//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPUNSUPPORTEDHEADER_H
#define CORE_SIPUNSUPPORTEDHEADER_H


#include "SipHeader.h"

class SipUnsupportedHeader : public SipHeader {

public:

    SipUnsupportedHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Unsupported";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::UNSUPPORTED;
    }

};


#endif //CORE_SIPUNSUPPORTEDHEADER_H
