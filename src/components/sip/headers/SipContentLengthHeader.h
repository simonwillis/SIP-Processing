//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPCONTENTLENGTHHEADER_H
#define CORE_SIPCONTENTLENGTHHEADER_H


#include "SipHeader.h"

class SipContentLengthHeader : public SipHeader {

public:

    SipContentLengthHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

        contentLength = this->getIntValue();
    }

    virtual const char *getFieldName() override {
        return "Content-Length";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::CONTENT_LENGTH;
    }

    int getContentLength() { return contentLength; }

private:
    int contentLength;

};


#endif //CORE_SIPCONTENTLENGTHHEADER_H
