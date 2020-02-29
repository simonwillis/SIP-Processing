//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIAALLOCATERESPONSE_H
#define MEDIA_MEDIAALLOCATERESPONSE_H


#include "MediaResponse.h"
#include "../definitions.h"

class MediaAllocateResponse : public MediaResponse {

public:

    MediaAllocateResponse(int dialogueId, const uint32_t callbackId, uint32_t resourceId, std::string answer, int errorCode, const char * errorMessage)
            : MediaResponse(dialogueId, callbackId, errorCode, errorMessage), resourceId(resourceId), answer(answer) {

        //fprintf(stderr, "MediaAllocateResponse::CTOR with properties\n");

    }

    const Method getMethod() override {
        return Method::ALLOCATE;
    }

    Json::Value getData() override {
        Json::Value data;
        data["resourceId"] = resourceId;
        data["offerAnswer"] = answer;
        return data;
    }

    uint32_t getResourceId() { return resourceId; }
    std::string getAnswer() { return answer; }


protected:

    bool hasData() override { return true; }


private:

    uint32_t resourceId;
    std::string answer;

};


#endif //MEDIA_MEDIAALLOCATERESPONSE_H
