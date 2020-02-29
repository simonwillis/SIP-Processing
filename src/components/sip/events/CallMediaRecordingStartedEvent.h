//
// Created by Simon Willis on 08/06/2016.
//

#ifndef TESTSESSIONMANAGER_CALLMEDIARECORDEVENT_H
#define TESTSESSIONMANAGER_CALLMEDIARECORDEVENT_H


#include "CallEvent.h"

class CallMediaRecordingStartedEvent : public CallEvent {

public:

    CallMediaRecordingStartedEvent(uint32_t channelId, std::string callId, bool start, int errorCode = 0, std::string errorMessage = std::string())
            : CallEvent(channelId, callId, errorCode, errorMessage) {

    }

    virtual CallEventType getEventType() override {
        return CallEventType::MEDIA_RECORDING_STARTED;
    }

    virtual const char *getEventTypeName() override { return "Recording Start"; };

};


#endif //TESTSESSIONMANAGER_CALLMEDIARECORDEVENT_H
