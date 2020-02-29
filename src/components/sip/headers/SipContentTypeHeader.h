//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPCONTENTTYPEHEADER_H
#define CORE_SIPCONTENTTYPEHEADER_H


#include "SipHeader.h"

class SipContentTypeHeader : public SipHeader {

public:

    SipContentTypeHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

        if (buffer_contains(headerValue, "XML")) bodyType = SipMessageBody::BodyType::XML;
        else if (buffer_contains(headerValue, "SDP")) bodyType = SipMessageBody::BodyType::SDP;
        else if (buffer_contains(headerValue, "MULTIPLE")) bodyType = SipMessageBody::BodyType::MULTIPLE;
        else if (buffer_contains(headerValue, "BOUNDARY")) bodyType = SipMessageBody::BodyType::BOUNDARY;
        else bodyType = SipMessageBody::BodyType::UNKNOWN;
    }

    virtual const char *getFieldName() override {
        return "Content-Type";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::CONTENT_TYPE;
    }

    SipMessageBody::BodyType getContentType() {
        return bodyType;
    }

private:
    SipMessageBody::BodyType bodyType;

};


#endif //CORE_SIPCONTENTTYPEHEADER_H
