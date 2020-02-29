//
// Created by Simon Willis on 22/05/2016.
//

#ifndef CORE_CALLRECORDINGSTARTEDNOTIFICATION_H
#define CORE_CALLRECORDINGSTARTEDNOTIFICATION_H

#include "Notification.hpp"

class CallRecordingStartedNotification : public Notification {

public:
    CallRecordingStartedNotification(const uint32_t channelId, const int errorCode, const char * errorMessage = "OK", const int dialogueId = 0)
            : Notification("callRecordStart", channelId, errorCode, errorMessage, dialogueId) {
        fprintf(stderr, "CallRecordingStartedNotification created with channelId as %u\n", channelId);
    }

};

#endif //CORE_CALLRECORDINGSTARTEDNOTIFICATION_H
