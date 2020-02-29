//
// Created by Simon Willis on 31/05/2016.
//

#ifndef SIP_SIPSYSTEMUNAVAILABLERESPONSE_H
#define SIP_SIPSYSTEMUNAVAILABLERESPONSE_H


#include "SipResponse.h"

class SipServiceUnavailableResponse : SipResponse {

    SipServiceUnavailableResponse(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers)
            : SipResponse(propertiesLine, headers) {

    }

    virtual const SipMessageType getMessageType() override {
        return SipMessageType::SERVICE_UNAVAILABLE;
    }

    virtual const uint32_t getResponseCode() override {
        return 503;
    }

    virtual const char *getResponseDescription() override {
        return "Service Unavailable";
    }
};


#endif //SIP_SIPSYSTEMUNAVAILABLERESPONSE_H
