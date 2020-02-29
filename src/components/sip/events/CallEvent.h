//
// Created by Simon Willis on 24/05/2016.
//

#ifndef TESTSIPPARSING_CALLEVENT_H
#define TESTSIPPARSING_CALLEVENT_H

#include <string>

#include "../../../../lib/json/json.h"
#include "../messages/SipMessageBody.h"

class CallEvent {

public:
    enum class CallEventType {
        UNKNOWN = 0x00,
        NEWCALL = 0x01,
        CALLANSWERED = 0x02,
        CALLCLEARED = 0x03,
        //MEDIA_OFFER = 0x11,
        MEDIA_STATS = 0x12,
        MEDIA_ERROR = 0x13,
        MEDIA_RECORDING_STARTED = 0x21,
        MEDIA_RECORDING_STOPPED = 0x22,
        MEDIA_PLAY_STARTED = 0x23,
        MEDIA_PLAY_STOPPED = 0x24,
        MEDIA_DTMF = 0x25
    };

    CallEvent(uint32_t channelId, std::string callId, int errorCode = 0, std::string errorMessage = std::string())
            : channelId(channelId), callId(callId), errorCode(errorCode), errorMessage(errorMessage) {

    }

    virtual CallEventType getEventType() = 0;
    virtual const char * getEventTypeName() = 0;

    uint32_t getChannelId() { return channelId; }
    std::string getCallId() { return callId; }
    int getErrorCode() { return errorCode; }
    std::string getErrorMessage() { return errorMessage; }
    int getDialogueId() { return dialogueId; }
//    bool hasBody(SipMessageBody::BodyType bodyType) { return bodies.find(bodyType) != bodies.end(); }
//    std::string getBody(SipMessageBody::BodyType bodyType) {
//        std::map<SipMessageBody::BodyType, std::string>::iterator it = bodies.find(bodyType);
//        if (it != bodies.end()) {
//            return (*it).second;
//        }
//        return std::string();
//    }
    //std::map<SipMessageBody::BodyType, std::string> getBodies() { return bodies; };

protected:

    CallEvent() { }

    uint32_t channelId = 0;
    std::string callId;
    int errorCode = 0;
    std::string errorMessage = std::string("OK");
    int dialogueId = 0;
    //std::map<SipMessageBody::BodyType, std::string> bodies;
};


#endif //TESTSIPPARSING_CALLEVENT_H
