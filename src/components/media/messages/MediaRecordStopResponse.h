//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIASTOPRECORDRESPONSE_H
#define MEDIA_MEDIASTOPRECORDRESPONSE_H


#include "MediaResponse.h"

class MediaRecordStopResponse : public MediaResponse {

public:

    MediaRecordStopResponse(int dialogueId, const uint32_t callbackId, uint32_t  resourceid, const char * filename, const char * systemReference, int errorCode, const char * errorMessage)
            : MediaResponse(dialogueId, callbackId, errorCode, errorMessage)
            , resourceId(resourceid), filename(filename), systemReference(systemReference) {

    }

    MediaRecordStopResponse(Json::Value data, int dialogueId, const uint32_t callbackId, int errorCode, const char * errorMessage)
            : MediaResponse(dialogueId, callbackId, errorCode, errorMessage) {

        resourceId = data.get("resourceId", 0).asUInt();
        filename = std::string(data.get("filename", "").asCString());
        systemReference = std::string(data.get("systemReference", "").asCString());
    }

    const Method getMethod() override {
        return Method::RECORD_STOP;
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


#endif //MEDIA_MEDIASTOPRECORDRESPONSE_H
