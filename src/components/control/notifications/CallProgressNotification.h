//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_CALLPROGRESSNOTIFICATION_H
#define CORE_CALLPROGRESSNOTIFICATION_H

#include "Notification.hpp"

class CallProgressNotification : public Notification {

public:
    CallProgressNotification(uint32_t channelId, int errorCode = 0, const char * errorMessage = "OK", int dialogueId = 0)
            : Notification("callProgress", channelId, errorCode, errorMessage, dialogueId) {
    }

};

#endif //CORE_CALLPROGRESSNOTIFICATION_H
