//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIARELEASERESPONSE_H
#define MEDIA_MEDIARELEASERESPONSE_H


#include "MediaResponse.h"

class MediaReleaseResponse : public MediaResponse {

public:

    MediaReleaseResponse(Json::Value data, int dialogueId, const uint32_t callbackId, int errorCode, const char * errorMessage)
            : MediaResponse(dialogueId, callbackId, errorCode, errorMessage) {

        resourceId = data.get("resourceId", 0).asUInt();
    }

    MediaReleaseResponse(int dialogueId, const uint32_t callbackId, uint32_t resourceId, int errorCode, const char * errorMessage)
            : MediaResponse(dialogueId, callbackId, errorCode, errorMessage), resourceId(resourceId) {

        //fprintf(stderr, "MediaAllocateResponse::CTOR with properties\n");

    }

    const Method getMethod() override {
        return Method::RELEASE;
    }

protected:

    bool hasData() override { return true; }

    Json::Value getData() override {
        Json::Value data;
        data["resourceId"] = resourceId;
        return data;
    }

private:

    uint32_t resourceId;

};


#endif //MEDIA_MEDIARELEASERESPONSE_H
