//
// Created by Simon Willis on 31/05/2016.
//

#ifndef SIP_SIPREQUEST_H
#define SIP_SIPREQUEST_H


#include "SipMessage.h"

class SipRequest : public SipMessage {

public:

    SipRequest();
    SipRequest(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers, std::map<SipMessageBody::BodyType, std::string> bodies = std::map<SipMessageBody::BodyType, std::string>());

    virtual const SipMessageType getRequestType() = 0;

    /// @brief All Requests have a METHOD
    /// @return the request's method
    virtual const char * getRequestMethod() = 0;

    const char * getTypeDescriptionString() override {
        return "Request";
    }

    const char * getSubTypeDescriptionString() override {
        return getRequestMethod();
    }

    virtual const bool supported() override = 0;

    virtual const bool isResponse() override {
        return false;
    }

    virtual const bool isRequest() override {
        return true;
    }

    std::string buildOkResponse(std::string localTag);

protected:

    std::stringstream buildStandardBaseResponse(const char * method, std::string localTag);
    std::string buildStandardFullResponse(const char * method, std::string localTag);

};


#endif //SIP_SIPREQUEST_H
