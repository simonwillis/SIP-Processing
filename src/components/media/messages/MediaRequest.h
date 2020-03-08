//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIAREQUEST_H
#define MEDIA_MEDIAREQUEST_H


#include "MediaMessage.h"

class MediaRequest : public MediaMessage {

public:

    MediaRequest(int dialogueId, uint32_t callbackId)
            : MediaMessage(dialogueId, callbackId) {

    }

    virtual const MediaMessage::Method getMethod() override = 0;
//
//public:
//
//    static MediaRequest * build_request(const char * message);


protected:

    virtual const Type getType() override {
        return Type::REQUEST;
    }

    virtual bool hasStatus() override {
        return false;
    }

    virtual Json::Value getData() override {
        return Json::Value::nullRef;
    }

    virtual Json::Value getStatus() override {
        return Json::Value::nullRef;
    }

    MediaRequest() {
        // shouldn't this be private
    }


};


#endif //MEDIA_MEDIAREQUEST_H
