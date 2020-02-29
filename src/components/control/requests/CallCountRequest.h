//
// Created by Simon Willis on 20/05/2016.
//

#ifndef CORE_CALLCOUNTREQUEST_H_H
#define CORE_CALLCOUNTREQUEST_H_H

#include "Request.h"

class CallCountRequest : public Request {
public:
    CallCountRequest(int dialogueId)
            : Request(dialogueId) { }

    ~CallCountRequest() {};

    const RequestMethod getMethod() override { return Request::INFO_CALLCOUNT; };
    const char * getMethodName() override { return "callCount"; };

private:
    CallCountRequest() {};

};


#endif //CORE_CALLCOUNTREQUEST_H_H
