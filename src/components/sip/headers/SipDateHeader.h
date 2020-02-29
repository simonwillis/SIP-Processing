//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPDATEHEADER_H
#define CORE_SIPDATEHEADER_H


#include "SipHeader.h"

class SipDateHeader : public SipHeader {

public:

    SipDateHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Date";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::DATE;
    }

};


#endif //CORE_SIPDATEHEADER_H
