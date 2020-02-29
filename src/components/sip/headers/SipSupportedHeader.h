//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPSUPPORTEDHEADER_H
#define CORE_SIPSUPPORTEDHEADER_H


#include "SipHeader.h"

class SipSupportedHeader : public SipHeader {

public:

    SipSupportedHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Supported";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::SUPPORTED;
    }

};


#endif //CORE_SIPSUPPORTEDHEADER_H
