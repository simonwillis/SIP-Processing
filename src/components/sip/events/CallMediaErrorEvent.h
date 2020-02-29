//
// Created by Simon Willis on 24/05/2016.
//

#ifndef TESTSIPPARSING_MEDIAERROREVENT_H
#define TESTSIPPARSING_MEDIAERROREVENT_H


#include "CallEvent.h"

class CallMediaErrorEvent : public CallEvent {

public:

    virtual CallEventType getEventType() override {
        return CallEventType::MEDIA_ERROR;
    }

    virtual const char *getEventTypeName() override {
        return "Media Error";
    }

};


#endif //TESTSIPPARSING_MEDIAERROREVENT_H
