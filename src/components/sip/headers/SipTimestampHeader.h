//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPTIMESTAMPHEADER_H
#define CORE_SIPTIMESTAMPHEADER_H


#include "SipHeader.h"

class SipTimestampHeader : public SipHeader {

public:

    SipTimestampHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Timestamp";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::TIMESTAMP;
    }

};


#endif //CORE_SIPTIMESTAMPHEADER_H
