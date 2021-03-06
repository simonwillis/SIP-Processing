//
// Created by Simon Willis on 31/05/2016.
//

#ifndef SIP_MESSAGES_SIPRINGINGRESPONSE_H
#define SIP_MESSAGES_SIPRINGINGRESPONSE_H


#include "SipResponse.h"

class SipRingingResponse : SipResponse {

    SipRingingResponse(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers)
            : SipResponse(propertiesLine, headers) {

    }

    virtual const SipMessageType getMessageType() override {
        return SipMessageType::RINGING;
    }

    virtual const uint32_t getResponseCode() override {
        return 180;
    }

    virtual const char *getResponseDescription() override {
        return "Ringing";
    }

};


#endif //SIP_MESSAGES_SIPRINGINGRESPONSE_H
