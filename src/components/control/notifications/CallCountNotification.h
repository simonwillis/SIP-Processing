//
// Created by Simon Willis on 20/05/2016.
//

#ifndef CORE_CALLCOUNTNOTIFICATION_H
#define CORE_CALLCOUNTNOTIFICATION_H

#include "Notification.hpp"

class CallCountNotification : public Notification {

public:
    CallCountNotification(int callCount, int errorCode = 0, const char * errorMessage = "OK", int dialogueId = 0)
            : Notification("callCount", 0, errorCode, errorMessage, dialogueId)
            , callCount(callCount) {
    }

protected:
    bool hasData() { return true; }

    Json::Value getData() {
        Json::Value data;
        data["callCount"] = callCount;
        return data;
    }

private:
    int callCount;

};

#endif //CORE_CALLCOUNTNOTIFICATION_H
