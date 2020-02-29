//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_CALLCLEARREQUEST_H
#define CORE_CALLCLEARREQUEST_H

#include <string>

#include "Request.h"

class CallClearRequest : public Request {
public:
    CallClearRequest(Json::Value & root, int dialogueId)
            : Request(dialogueId) {
        channelId = root.get("channelId", 0).asInt();
        clearingCode = root.get("clearingCode", 200).asInt();
    }

    ~CallClearRequest() {};

    const RequestMethod getMethod() override { return Request::CALL_CLEAR; }
    const char * getMethodName() override { return "callClear"; };
    const int getChannelId() override { return channelId; }

    const int getClearingCode() { return clearingCode; }

private:
    CallClearRequest() {};
    int channelId;
    int clearingCode;
};

#endif //CORE_CALLCLEARREQUEST_H
