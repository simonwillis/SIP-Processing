//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPREQUIREAFTERHEADER_H
#define CORE_SIPREQUIREAFTERHEADER_H


#include "SipHeader.h"

class SipRequireAfterHeader : public SipHeader {

public:

    SipRequireAfterHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Require-After";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::REQUIRE_AFTER;
    }

};


#endif //CORE_SIPREQUIREAFTERHEADER_H
