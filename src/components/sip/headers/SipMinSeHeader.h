//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPMINSEHEADER_H
#define CORE_SIPMINSEHEADER_H


#include "SipHeader.h"

class SipMinSeHeader : public SipHeader {

public:

    SipMinSeHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Min-SE";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::MINSE;
    }

};


#endif //CORE_SIPMINSEHEADER_H
