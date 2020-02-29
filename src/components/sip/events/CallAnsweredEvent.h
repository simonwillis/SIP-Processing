//
// Created by Simon Willis on 24/05/2016.
//

#ifndef TESTSIPPARSING_CALLANSWEREDEVENT_H
#define TESTSIPPARSING_CALLANSWEREDEVENT_H


#include "CallEvent.h"

class CallAnsweredEvent : public CallEvent {

public:

    CallAnsweredEvent(uint32_t channelId, std::string callId)
            : CallEvent(channelId, callId) {

    }

    virtual CallEventType getEventType() override {
        return CallEventType::CALLANSWERED;
    }

    virtual const char *getEventTypeName() override {
        return "Call Answered";
    }


};


#endif //TESTSIPPARSING_CALLANSWEREDEVENT_H
