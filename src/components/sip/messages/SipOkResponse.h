//
// Created by Simon Willis on 31/05/2016.
//

#ifndef TESTMAPPEDBUFFERS_SIPOKRESPONSE_H
#define TESTMAPPEDBUFFERS_SIPOKRESPONSE_H


#include "SipResponse.h"

class SipOkResponse : public SipResponse {

public:

    SipOkResponse(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers)
            : SipResponse(propertiesLine, headers) {

    }

    virtual const SipMessageType getResponseType() override {
        return SipMessageType::OK;
    }

    virtual const uint32_t getResponseCode() override {
        return 200;
    }

    virtual const char *getResponseDescription() override {
        return "OK";
    }


};


#endif //TESTMAPPEDBUFFERS_SIPOKRESPONSE_H
