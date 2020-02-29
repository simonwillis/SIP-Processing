//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIASTARTRECORDREQUEST_H
#define MEDIA_MEDIASTARTRECORDREQUEST_H


#include "MediaRequest.h"

class MediaRecordStartRequest : public MediaRequest {


public:

    MediaRecordStartRequest(int dialogueId, const uint32_t callbackId, uint32_t resourceId, const char * filename)
            : MediaRequest(dialogueId, callbackId), resourceId(resourceId), filename(filename) {

    }

    MediaRecordStartRequest(const uint32_t callbackId, int dialogueId, Json::Value data)
            : MediaRequest(dialogueId, callbackId) {

        resourceId = data.get("resourceId", 0).asUInt();

        // An empty filename allows the media process to create a unique local filename
        filename = std::string(data.get("filename", "").asCString());
    }

    const Method getMethod() override {
        return Method::RECORD_START;
    }

    const uint32_t getResourceId() { return resourceId; }
    const std::string getFilename() { return filename; }

protected:

    bool hasData() override { return true; }

    Json::Value getData() override {
        Json::Value data;
        data["resourceId"] = resourceId;
        data["filename"] = filename.c_str();

        return data;
    }

private:
    uint32_t resourceId;
    std::string filename;

};


#endif //MEDIA_MEDIASTARTRECORDREQUEST_H
