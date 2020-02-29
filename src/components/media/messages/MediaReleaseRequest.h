//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIARELEASEREQUEST_H
#define MEDIA_MEDIARELEASEREQUEST_H


#include "MediaRequest.h"

class MediaReleaseRequest : public MediaRequest {

public:

    MediaReleaseRequest(int dialogueId, const uint32_t callbackId, uint32_t resourceId, const char * ownerReference)
            : MediaRequest(dialogueId, callbackId), resourceId(resourceId), ownerReference(ownerReference) {

    }

    MediaReleaseRequest(int dialogueId, const uint32_t callbackId, Json::Value data)
            : MediaRequest(dialogueId, callbackId)
            , resourceId(data.get("resourceId", 0).asUInt()) {

    }

    const Method getMethod() override {
        return Method::RELEASE;
    }

    const std::string getOwnerReference() {
        return ownerReference;
    }

    const uint32_t getResourceId() {
        return resourceId;
    }

protected:

    bool hasData() override { return true; }

    Json::Value getData() override {
        Json::Value data;

        data["ownerReference"] = ownerReference.c_str();
        data["resourceId"] = resourceId;

        return data;
    }

private:

    uint32_t resourceId;
    std::string ownerReference;

};


#endif //MEDIA_MEDIARELEASEREQUEST_H
