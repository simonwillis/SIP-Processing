//
// Created by Simon Willis on 10/06/2016.
//

#ifndef MEDIA_MEDIAREGISTERRESPONSE_H
#define MEDIA_MEDIAREGISTERRESPONSE_H


#include "MediaResponse.h"

class MediaRegisterResponse : public MediaResponse {

public:
    MediaRegisterResponse(int dialogueId, const uint32_t callbackId, int errorCode, const char * errorMessage)
            : MediaResponse(dialogueId, callbackId, errorCode, errorMessage) {

    }

    const Method getMethod() override {
        return Method::REGISTER;
    }

};


#endif //MEDIA_MEDIAREGISTERRESPONSE_H
