//
// Created by Simon Willis on 19/05/2016.
//

#ifndef CORE_LOGINREQUEST_H
#define CORE_LOGINREQUEST_H

#include "Request.h"

class LoginRequest : public Request {
public:
    LoginRequest(Json::Value & root, int dialogueId)
            : Request(dialogueId) { }

    ~LoginRequest() {};

    const RequestMethod getMethod() override { return Request::SYSTEM_LOGIN; };
    const char * getMethodName() override { return "login"; };

private:
    LoginRequest() {};

};


#endif //CORE_LOGINREQUEST_H
