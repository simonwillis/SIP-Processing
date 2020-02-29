//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPTOHEADER_H
#define CORE_SIPTOHEADER_H


#include "SipHeader.h"

class SipToHeader : public SipHeader {

public:

    SipToHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "To";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::TO;
    }

    std::string getOriginalToField() {
        return headerValue.toString();
    }
};


#endif //CORE_SIPTOHEADER_H
