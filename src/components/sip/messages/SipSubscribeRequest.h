//
// Created by Simon Willis on 24/05/2016.
//

#ifndef SIP_MESSAGES_SIPSUBSCRIBEMESSAGE_H
#define SIP_MESSAGES_SIPSUBSCRIBEMESSAGE_H


#include "SipMessage.h"
#include "SipRequest.h"

class SipSubscribeRequest : public SipRequest {

public:

    SipSubscribeRequest(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers)
            : SipRequest(propertiesLine, headers) {

    }

    virtual const SipMessageType getRequestType() override {
        return SipMessageType::SUBSCRIBE;
    }

    virtual const char *getRequestMethod() override {
        return "SUBSCRIBE";
    }

    virtual const bool supported() override {
        return true;
    }

    std::string buildSubscriptionAcceptedResponse(std::string localTag) {
        std::stringstream ss = buildStandardBaseResponse("200 OK", localTag);

        return ss.str();
    };

    std::string buildSubscriptionRejectedResponse(std::string localTag) {
        std::stringstream ss = buildStandardBaseResponse("200 OK", localTag);

        return ss.str();
    };
};


#endif //SIP_MESSAGES_SIPSUBSCRIBEMESSAGE_H
