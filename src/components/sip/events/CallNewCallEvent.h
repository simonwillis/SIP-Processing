//
// Created by Simon Willis on 24/05/2016.
//

#ifndef TESTSIPPARSING_NEWCALLEVENT_H
#define TESTSIPPARSING_NEWCALLEVENT_H


#include "CallEvent.h"
#include "../../media/definitions.h"
#include "../../utilities/base64.h"

class CallNewCallEvent : public CallEvent {

public:

    CallNewCallEvent(uint32_t channelId, std::string callId, std::string tag, std::string to, std::string from, sip_headers_t headers, std::map<SipMessageBody::BodyType, std::string> bodies)
            : CallEvent(channelId, callId), tag(tag), to(to), from(from), headers(headers), bodies(bodies) {
        fprintf(stderr, "CallNewCallEvent::CTOR [channelId=%d] [callId=%s] [to=%s] [from=%s]\n", channelId, callId.c_str(), to.c_str(), from.c_str());
    }

    virtual CallEventType getEventType() override {
        return CallEvent::CallEventType::NEWCALL;
    }

    virtual const char *getEventTypeName() override {
        return "New Call";
    }

    std::string getTag() { return tag; }
    std::string getTo() { return to; }
    std::string getFrom() { return from; }
    sip_headers_t getHeaders() { return headers; }
    bool hasBody(SipMessageBody::BodyType bodyType) { return bodies.find(bodyType) != bodies.end(); }
    std::string getBody(SipMessageBody::BodyType bodyType) {
        std::map<SipMessageBody::BodyType, std::string>::iterator it = bodies.find(bodyType);
        if (it != bodies.end()) {
            return (*it).second;
        }
        return std::string();
    }
    std::map<SipMessageBody::BodyType, std::string> getBodies() { return bodies; }

private:
    std::string tag;
    std::string to;
    std::string from;
    sip_headers_t headers;
    std::map<SipMessageBody::BodyType, std::string> bodies;

};


#endif //TESTSIPPARSING_NEWCALLEVENT_H
