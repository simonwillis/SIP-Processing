//
// Created by Simon Willis on 24/05/2016.
//

#ifndef TESTSIPPARSING_CALLCLEAREDEVENT_H
#define TESTSIPPARSING_CALLCLEAREDEVENT_H


#include "CallEvent.h"

class CallClearedEvent : public CallEvent {

public:

    CallClearedEvent(uint32_t channelId, std::string callId, int clearingCode = 200)
            : CallEvent(channelId, callId), clearingCode(clearingCode) {

    }

    virtual CallEventType getEventType() override {
        return CallEventType::CALLCLEARED;
    }

    virtual const char *getEventTypeName() override {
        return "Call Cleared";
    }

    int getClearingCode() { return clearingCode; }

private:
    int clearingCode;

};


#endif //TESTSIPPARSING_CALLCLEAREDEVENT_H
