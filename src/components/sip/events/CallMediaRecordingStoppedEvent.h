//
// Created by Simon Willis on 09/06/2016.
//

#ifndef TESTSESSIONMANAGER_CALLMEDIARECORDSTART_H
#define TESTSESSIONMANAGER_CALLMEDIARECORDSTART_H

#include "CallEvent.h"

class CallMediaRecordingStoppedEvent : public CallEvent {

public:

    CallMediaRecordingStoppedEvent(uint32_t channelId, std::string callId, bool start, int errorCode = 0, std::string errorMessage = std::string())
            : CallEvent(channelId, callId, errorCode, errorMessage) {

    }

    virtual CallEventType getEventType() override {
        return CallEventType::MEDIA_RECORDING_STOPPED;
    }

    virtual const char *getEventTypeName() override { return "Recording Stopped"; };


};


#endif //TESTSESSIONMANAGER_CALLMEDIARECORDSTART_H
