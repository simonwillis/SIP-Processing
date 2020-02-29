//
// Created by Simon Willis on 06/07/2016.
//

#ifndef UDPSEND_CALLINITIATEREQUEST_H
#define UDPSEND_CALLINITIATEREQUEST_H


#include "Request.h"
#include "../../sip/headers/SipHeader.h"

class CallInitiateRequest : public Request {

public:
    CallInitiateRequest(Json::Value & root, int dialogueId)
            : Request(dialogueId) {
        to = std::string(root.get("to", "").asCString());
        from = std::string(root.get("from", "").asCString());
        body = std::string(root.get("body", "").asCString());
        tag = std::string(root.get("tag", "tag-not-set").asCString());

    }
    virtual const RequestMethod getMethod() override {
        return CALL_INITITIATE;
    }

    virtual const char *getMethodName() override {
        return "makeCall";
    }

private:
    std::string to;
    std::string from;
    std::string body;
    std::string tag;
    sip_headers_t headers;


};


#endif //UDPSEND_CALLINITIATEREQUEST_H
