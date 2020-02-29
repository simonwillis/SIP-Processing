//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPCALLIDHEADER_H
#define CORE_SIPCALLIDHEADER_H


#include "SipHeader.h"

class SipCallIdHeader : public SipHeader {

public:

    SipCallIdHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {
        // A Call-ID header value is a simple string that can be used as-is. No need to process
    }

    virtual const char *getFieldName() override {
        return "Call-ID";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::CALL_ID;
    }

    std::string getCallId() {
        return headerValue.toString();
    }
};


#endif //CORE_SIPCALLIDHEADER_H
