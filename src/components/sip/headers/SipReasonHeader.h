//
// Created by Simon Willis on 07/06/2016.
//

#ifndef TESTSESSIONMANAGER_SIPREASONHEADER_H
#define TESTSESSIONMANAGER_SIPREASONHEADER_H


#include <utmpx.h>
#include "SipHeader.h"

class SipReasonHeader : public SipHeader {

//    Example Reason Headers
//    Reason: SIP ;cause=200 ;text="Call completed elsewhere"
//    Reason: Q.850 ;cause=16 ;text="Terminated"
//    Reason: SIP ;cause=600 ;text="Busy Everywhere"
//    Reason: SIP ;cause=580 ;text="Precondition Failure"
//
//    See: https://tools.ietf.org/html/rfc3326

public:

    SipReasonHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

        //TODO Parse out reason code information

    }

    virtual const char *getFieldName() override {
        return "Reason";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::REASON;
    }

    int getCause() { return cause; }
    std::string getCauseText() { return causeText.toString(); }
    std::string getProtocol() { return protocol.toString(); }
    bool isSip() { return buffer_equals(protocol, "SIP"); }
    bool isQ850() { return buffer_equals(protocol, "Q.850"); }

private:

    int cause;
    mapped_buffer_t causeText;
    mapped_buffer_t protocol;

};


#endif //TESTSESSIONMANAGER_SIPREASONHEADER_H
