//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_CALLCLEAREDNOTIFICATION_H
#define CORE_CALLCLEAREDNOTIFICATION_H

#include "Notification.hpp"

class CallClearedNotification : public Notification {

public:
    CallClearedNotification(uint32_t channelId, const int clearingCode, const int errorCode = 0, const char * errorMessage = "OK", int dialogueId = 0)
            : Notification("callCleared", channelId, errorCode, errorMessage, dialogueId)
            , clearingCode(clearingCode) {
    }

protected:
    bool hasData() { return true; }

    Json::Value getData() {
        Json::Value data;
        data["clearingCode"] = clearingCode;
        return data;
    }

private:
    int clearingCode;
};

#endif //CORE_CALLCLEAREDNOTIFICATION_H
