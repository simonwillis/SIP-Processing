//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIASTOPRECORDREQUEST_H
#define MEDIA_MEDIASTOPRECORDREQUEST_H


#include "MediaRequest.h"

class MediaRecordStopRequest : public MediaRequest {


public:

    MediaRecordStopRequest(int dialogueId, const uint32_t callbackId, uint32_t resourceId, int dummy = 0)
            : MediaRequest(dialogueId, callbackId), resourceId(resourceId) {

    }

    MediaRecordStopRequest(const uint32_t callbackId, int dialogueId, Json::Value data)
            : MediaRequest(dialogueId, callbackId) {

            resourceId = data.get("resourceId", 0).asUInt();
    }

    const Method getMethod() override {
        return Method::RECORD_STOP;
    }

    const uint32_t getResourceId() { return resourceId; }

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


#endif //MEDIA_MEDIASTOPRECORDREQUEST_H
