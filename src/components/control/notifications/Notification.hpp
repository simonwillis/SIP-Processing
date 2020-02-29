//
// Created by Simon Willis on 20/05/2016.
//

#ifndef CORE_RESPONSE_H
#define CORE_RESPONSE_H

#include <string>

#include "../../../../lib/json/json.h"

class Notification {
public:
    Notification() {}
    ~Notification() {}

    virtual std::string toJson() {
        Json::Value root;
        root["type"] = "notification";
        root["channelId"] = channelId;
        root["method"] = method.c_str();
        root["errorCode"] = errorCode;
        root["errorMessage"] = errorMessage.c_str();
        root["dialogueId"] = dialogueId;
        root["data"] = getData();

        Json::FastWriter writer;

        return writer.write(root);
    }

protected:
    Notification(const char * method, int channelId, int errorCode, const char * errorMessage, int dialogueId)
            : method(method), channelId(channelId), errorCode(errorCode), errorMessage(errorMessage), dialogueId(dialogueId) { }

    virtual bool hasData() { return false; }
    virtual Json::Value getData() { return emptyDataSet; }

    std::string method;
    int channelId;
    int errorCode;
    std::string errorMessage;
    int dialogueId;

private:
    const Json::Value emptyDataSet;

};


#endif //CORE_RESPONSE_H
