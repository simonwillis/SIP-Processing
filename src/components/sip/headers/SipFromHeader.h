//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPFROMHEADER_H
#define CORE_SIPFROMHEADER_H


#include "SipHeader.h"

class SipFromHeader : public SipHeader {

public:

    SipFromHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "From";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::FROM;
    }

    std::string getOriginalFromField() {
        return headerValue.toString();
    }

};


#endif //CORE_SIPFROMHEADER_H
