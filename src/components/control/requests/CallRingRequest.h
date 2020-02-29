//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_CALLRINGREQUEST_H
#define CORE_CALLRINGREQUEST_H

#include <string>

#include "Request.h"

class CallRingRequest : public Request {
public:
    CallRingRequest(Json::Value & root, int dialogueId)
            : Request(dialogueId) { }

    ~CallRingRequest() {};

    const RequestMethod getMethod() override { return Request::CALL_RING; }
    const char * getMethodName() override { return "sendRingback"; };
    const int getChannelId() override { return channelId; }

private:
    CallRingRequest() {};
    int channelId;

};

#endif //CORE_CALLRINGREQUEST_H
