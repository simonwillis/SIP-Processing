//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_CALLANSWERREQUEST_H
#define CORE_CALLANSWERREQUEST_H

#include <string>

#include "Request.h"

class CallAnswerRequest : public Request {
public:
    CallAnswerRequest(Json::Value & root, int dialogueId)
            : Request(dialogueId) {
        channelId = root.get("channelId", 0).asInt();
        tag = std::string(root.get("tag", "tag-not-set").asCString());
        contactField = std::string(root.get("contact", "").asCString());
    }

    ~CallAnswerRequest() {};

    const RequestMethod getMethod() override { return Request::CALL_ANSWER; };
    const char * getMethodName() override { return "callAnswer"; };
    const int getChannelId() override { return channelId; }

    const std::string getTag() { return tag; }
    const std::string getContactField() { return contactField; }

private:
    CallAnswerRequest() {};
    int channelId;
    std::string tag;
    std::string contactField;
};

#endif //CORE_CALLANSWERREQUEST_H
