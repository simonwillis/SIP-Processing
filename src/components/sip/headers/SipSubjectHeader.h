//
// Created by Simon Willis on 08/06/2016.
//

#ifndef TESTSESSIONMANAGER_SIPSUBJECTHEADER_H
#define TESTSESSIONMANAGER_SIPSUBJECTHEADER_H


#include <utmpx.h>
#include "SipHeader.h"

class SipSubjectHeader : public SipHeader {

public:

    SipSubjectHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Subject";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::SUBJECT;
    }


};


#endif //TESTSESSIONMANAGER_SIPSUBJECTHEADER_H
