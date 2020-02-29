//
// Created by Simon Willis on 25/05/2016.
//

#ifndef SIP_HEADERS_SIPASSERTEDIDENTITYHEADER_H
#define SIP_HEADERS_SIPASSERTEDIDENTITYHEADER_H


#include "SipHeader.h"

class SipAssertedIdentityHeader : public SipHeader {

public:

    SipAssertedIdentityHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "P-Asserted_Identity";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::P_ASSERTED_IDENTITY;
    }


};


#endif //SIP_HEADERS_SIPASSERTEDIDENTITYHEADER_H
