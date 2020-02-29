//
// Created by Simon Willis on 25/05/2016.
//

#ifndef TESTSIPPARSING_SIPEXTENDEDHEADER_H
#define TESTSIPPARSING_SIPEXTENDEDHEADER_H


#include "SipHeader.h"

class SipExtendedHeader : public SipHeader {

public:

    SipExtendedHeader(mapped_buffer_t fieldName, mapped_buffer_t headerValue)
            : SipHeader(headerValue), headerName(fieldName.toString()) {

    }

    virtual const char *getFieldName() override {
        return this->headerName.c_str();
    }

    virtual bool isExtended() override {
        return true;
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::EXTENDED;
    }

private:
    std::string headerName;

};


#endif //TESTSIPPARSING_SIPEXTENDEDHEADER_H
