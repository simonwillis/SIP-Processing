//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIAALLOCATEREQUEST_H
#define MEDIA_MEDIAALLOCATEREQUEST_H


#include "MediaRequest.h"
#include "../definitions.h"

class MediaAllocateRequest : public MediaRequest {


public:

    MediaAllocateRequest(const int dialogueId, const uint32_t callbackId, const std::string callId, const std::string sdp)
            : MediaRequest(dialogueId, callbackId), callId(callId), offer(sdp) { }

public:

    const Method getMethod() override {
        return Method::ALLOCATE;
    }

    virtual Json::Value getData() override {
        Json::Value data;
        data["callId"] = callId;
        data["sdp"] = offer;
        return data;
    }

    virtual Json::Value getStatus() override {
        return Json::Value::nullRef;
    }

    const std::string getCallId() { return callId; }

    const std::string getOffer() { return offer; }

protected:

    bool hasData() override { return true; }

private:

    std::string callId;
    std::string offer;

};


#endif //MEDIA_MEDIAALLOCATEREQUEST_H
