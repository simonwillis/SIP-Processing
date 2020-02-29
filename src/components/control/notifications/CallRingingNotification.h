//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_CALLRINGINGNOTIFICATION_H
#define CORE_CALLRINGINGNOTIFICATION_H

#include "Notification.hpp"

class CallRingingNotification : public Notification {

public:
    CallRingingNotification(uint32_t channelId, int errorCode = 0, const char *errorMessage = "OK", int dialogueId = 0)
            : Notification("callRinging", channelId, errorCode, errorMessage, dialogueId) {
    }

};

#endif //CORE_CALLRINGINGNOTIFICATION_H
