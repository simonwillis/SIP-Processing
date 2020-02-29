//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIARESPONSE_H
#define MEDIA_MEDIARESPONSE_H


#include "MediaMessage.h"

class MediaResponse : public MediaMessage {

protected:

    MediaResponse(int dialogueId, uint32_t callbackId, int errorCode, const char * errorMessage)
            : MediaMessage(dialogueId, callbackId), errorCode(errorCode), errorMessage(errorMessage) {

    }

    virtual bool hasStatus() override {
        return true;
    }

    Json::Value getStatus() override {
        Json::Value data;
        data["errorCode"] = getErrorCode();
        data["errorMessage"] = getErrorMessage();
        return data;
    }

public:
    virtual const Type getType() override {
        return Type::RESPONSE;
    }

    virtual const Method getMethod() override = 0;

    virtual Json::Value getData() override { return Json::Value::nullRef; };

    const int getErrorCode() { return errorCode; }

    const std::string getErrorMessage() { return errorMessage; };

private:
    const int errorCode;
    const std::string errorMessage;

};


#endif //MEDIA_MEDIARESPONSE_H
