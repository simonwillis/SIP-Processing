//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_CALLPROGRESSREQUEST_H
#define CORE_CALLPROGRESSREQUEST_H

#include <string>

#include "Request.h"

class CallProgressRequest : public Request {
public:
    CallProgressRequest(Json::Value & root, int dialogueId)
            : Request(dialogueId) {
        channelId = root.get("channelId", 0).asInt();
    }

    ~CallProgressRequest() {};

    const RequestMethod getMethod() override { return Request::CALL_PROGRESS; }
    const char * getMethodName() override { return "callProgress"; };
    const int getChannelId() override { return channelId; }

private:
    CallProgressRequest() {};
    int channelId;
};

#endif //CORE_CALLPROGRESSREQUEST_H
