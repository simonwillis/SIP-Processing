//
// Created by Simon Willis on 19/05/2016.
//

#ifndef CORE_CONTROLREQUEST_H
#define CORE_CONTROLREQUEST_H


#include "../../../../lib/json/json.h"


class Request {
public:
    enum RequestMethod {
        UNKNOWN = 0x00,

        SYSTEM_LOGIN = 0x01,
        SYSTEM_LOGINCHALLENGERESPONSE = 0x02,

        CALL_INITITIATE = 0x00,
        CALL_PROGRESS = 0x11,
        CALL_RING = 0x12,
        CALL_ANSWER = 0x13,
        CALL_CLEAR = 0x14,
        CALL_RECORD = 0x15,

        INFO_CALLCOUNT = 0x21,
        INFO_STATUS = 0x22
    };

    int getDialogueId() { return dialogueId; };

    static Request * build_request(const char * json);

    virtual const RequestMethod getMethod() = 0;
    virtual const char * getMethodName() = 0;
    virtual const int getChannelId() { return 0; };
    virtual const std::string getCallId() { return std::string(""); }


protected:
    Request(int dialogueId) : dialogueId(dialogueId) {};

    ~Request() {}

    Request() {}

protected:
    int dialogueId;


};


#endif //CORE_CONTROLREQUEST_H
