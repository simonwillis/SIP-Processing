//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_CALLANSWEREDNOTIFICATION_H
#define CORE_CALLANSWEREDNOTIFICATION_H

#include "Notification.hpp"

class CallConnectedNotification : public Notification {

public:
    CallConnectedNotification(uint32_t channelId, int errorCode = 0, const char *errorMessage = "OK", int dialogueId = 0)
            : Notification("callConnected", channelId, errorCode, errorMessage, dialogueId) {
    }

};

#endif //CORE_CALLANSWEREDNOTIFICATION_H
