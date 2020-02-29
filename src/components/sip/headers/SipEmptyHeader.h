//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPEMPTYHEADER_H
#define CORE_SIPEMPTYHEADER_H


#include "SipHeader.h"

class SipEmptyHeader : public SipHeader {

public:

    SipEmptyHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "X-Empty";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::EMPTY;
    }

};


#endif //CORE_SIPEMPTYHEADER_H
