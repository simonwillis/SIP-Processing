//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPCONTENTDISPOSITIONHEADER_H
#define CORE_SIPCONTENTDISPOSITIONHEADER_H


#include "SipHeader.h"

class SipContentDispositionHeader : public SipHeader {

public:

    SipContentDispositionHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Content-Disposition";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::CONTENT_DISPOSITION;
    }

};


#endif //CORE_SIPCONTENTDISPOSITIONHEADER_H
