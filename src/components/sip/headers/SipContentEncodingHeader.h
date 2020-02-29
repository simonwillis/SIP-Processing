//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPCONTENTENCODINGHEADER_H
#define CORE_SIPCONTENTENCODINGHEADER_H


#include "SipHeader.h"

class SipContentEncodingHeader : public SipHeader {

public:

    SipContentEncodingHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Content-Encoding";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::CONTENT_ENCODING;
    }

};


#endif //CORE_SIPCONTENTENCODINGHEADER_H
