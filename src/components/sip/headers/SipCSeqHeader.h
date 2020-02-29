//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPCSEQHEADER_H
#define CORE_SIPCSEQHEADER_H


#include "SipHeader.h"

class SipCSeqHeader : public SipHeader {

public:

    SipCSeqHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "C-Seq";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::CSEQ;
    }

    std::string getCSeq() {
        return headerValue.toString();
    }
};


#endif //CORE_SIPCSEQHEADER_H
