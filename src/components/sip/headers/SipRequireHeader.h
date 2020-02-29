//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPREQUIREHEADER_H
#define CORE_SIPREQUIREHEADER_H


#include "SipHeader.h"

class SipRequireHeader : public SipHeader {

public:

    SipRequireHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Require";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::REQUIRE;
    }

};


#endif //CORE_SIPREQUIREHEADER_H
