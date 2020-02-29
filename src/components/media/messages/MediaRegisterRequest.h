//
// Created by Simon Willis on 10/06/2016.
//

#ifndef MEDIA_MEDIAREGISTERREQUEST_H
#define MEDIA_MEDIAREGISTERREQUEST_H


#include "MediaRequest.h"

class MediaRegisterRequest : public MediaRequest {

public:

    MediaRegisterRequest(const char * identifier, int dialogueId, const uint32_t callbackId)
            : MediaRequest(dialogueId, callbackId), identifier(std::string(identifier)) {

    }

    MediaRegisterRequest(Json::Value & root, int dialogueId, const uint32_t callbackId)
            : MediaRequest(dialogueId, callbackId) {
            identifier = std::string(root.get("identifier", "unidentified").asCString());
    }

    const Method getMethod() override {
        return Method::REGISTER;
    }

protected:

    bool hasData() override { return true; }
    Json::Value getData() override {
        Json::Value data;
        data["identifier"] = identifier;
        return data;
    }

private:
    std::string identifier;

};


#endif //MEDIA_MEDIAREGISTERREQUEST_H
