//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPMAXFORWARDSHEADER_H
#define CORE_SIPMAXFORWARDSHEADER_H


#include "SipHeader.h"

class SipMaxForwardsHeader : public SipHeader {

public:

    SipMaxForwardsHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue), count(atoi(headerValue.toString().c_str())) {

    }

    virtual const char *getFieldName() override {
        return "Max-Forwards";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::MAX_FORWARDS;
    }

    const int getCount() { return count; }

private:
    const int count;
};


#endif //CORE_SIPMAXFORWARDSHEADER_H
