//
// Created by Simon Willis on 22/05/2016.
//

#ifndef CORE_CALLRECORDINGCOMPLETEDNOTIFICATION_H
#define CORE_CALLRECORDINGCOMPLETEDNOTIFICATION_H

#include "Notification.hpp"

class CallRecordingCompletedNotification : public Notification {

public:
    CallRecordingCompletedNotification(const uint32_t channelId, const int errorCode = 0, const char *errorMessage = "OK", const int dialogueId = 0)
            : Notification("callRecordEnd", channelId, errorCode, errorMessage, dialogueId) {
    }
};

#endif //CORE_CALLRECORDINGCOMPLETEDNOTIFICATION_H
