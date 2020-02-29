//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_CALLRECORDREQUEST_H
#define CORE_CALLRECORDREQUEST_H

#include <string>

#include "Request.h"

class CallRecordRequest : public Request {
public:
    CallRecordRequest(Json::Value & root, int dialogueId)
            : Request(dialogueId) {
        channelId = root.get("channelId", 0).asInt();
        filename = std::string(root.get("filename", "").asCString());
        start = (std::string(root.get("action", "start").asCString()).compare("start") == 0);
    }

    ~CallRecordRequest() {};

    const RequestMethod getMethod() override { return Request::CALL_RECORD; }
    const char * getMethodName() override { return "callRecord"; };
    const int getChannelId() override { return channelId; }
    const bool isStart() { return start; }

    std::string getFilename() { return filename; }

private:
    CallRecordRequest() {};
    int channelId;
    std::string filename;
    bool start;
};

#endif //CORE_CALLRECORDREQUEST_H
