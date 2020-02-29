//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIASTARTRECORDRESPONSE_H
#define MEDIA_MEDIASTARTRECORDRESPONSE_H


#include "MediaResponse.h"

class MediaRecordStartResponse : public MediaResponse {

public:

    MediaRecordStartResponse(int dialogueId, const uint32_t callbackId, uint32_t  resourceid, const char * filename, const char * systemReference, int errorCode, const char * errorMessage) :
            MediaResponse(dialogueId, callbackId, errorCode, errorMessage), resourceId(resourceid), filename(filename), systemReference(systemReference) {

    }

    MediaRecordStartResponse(Json::Value data, int dialogueId, const uint32_t callbackId, int errorCode, const char * errorMessage)
            : MediaResponse(dialogueId, callbackId, errorCode, errorMessage) {

        resourceId = data.get("resourceId", 0).asUInt();
        filename = std::string(data.get("filename", "").asCString());
        systemReference = std::string(data.get("systemReference", "").asCString());
    }

    const Method getMethod() override {
        return Method::RECORD_START;
    }

    const uint32_t getResourceId() { return resourceId; }
    const std::string getFilename() { return filename; }
    const std::string getSystemReference() { return systemReference; }

protected:

    bool hasData() override { return true; }

    Json::Value getData() override {
        Json::Value data;
        data["resourceId"] = resourceId;
        data["filename"] = filename.c_str();
        data["systemReference"] = systemReference.c_str();

        return data;
    }

private:

    uint32_t resourceId;
    std::string filename;
    // System Reference used to determine which unit the recording is on
    // This may need to be extended to allow the recipient to better locate the file for onward processing
    std::string systemReference;

};


#endif //MEDIA_MEDIASTARTRECORDRESPONSE_H
