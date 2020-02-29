//
// Created by Simon Willis on 24/05/2016.
//

#ifndef SIP_MESSAGES_SIPINVITEMESSAGE_H
#define SIP_MESSAGES_SIPINVITEMESSAGE_H


#include "SipMessage.h"
#include "SipRequest.h"

class SipInviteRequest : public SipRequest {

public:

    SipInviteRequest() {

    }

    SipInviteRequest(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers, std::map<SipMessageBody::BodyType, std::string> bodies)
        : SipRequest(propertiesLine, headers, bodies) {

    }

    virtual const SipMessageType getRequestType() override {
        return SipMessageType::INVITE;
    }

    virtual const char * getRequestMethod() override {
        return "INVITE";
    }

    virtual const bool supported() override {
        return false;
    }

    void isReinvite(bool flag);
    bool isReinvite();

    std::string buildTryingResponse(std::string localTag);
    std::string buildRingingResponse(std::string localTag);
    std::string buildAnswerResponse(std::string localTag, std::string sdpAnswer);
    std::string buildServiceUnavailableResponse(std::string localTag, uint32_t retryAfter);
    std::string buildMovedTemporarilyRespose(std::string localTag, std::string alternateAddress, uint32_t expiresAfter);

private:
    std::string sdp;
    std::string xml;
    bool reinviteFlag;

};


#endif //SIP_MESSAGES_SIPINVITEMESSAGE_H
