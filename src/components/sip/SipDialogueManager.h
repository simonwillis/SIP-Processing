//
// Created by Simon Willis on 03/06/2016.
//

#ifndef _SESSIONMANAGER_H
#define _SESSIONMANAGER_H


#include <map>
#include "SipDialogue.h"
#include "SipParser.h"
#include "messages/SipRequest.h"

class SipDialogueManager {

public:

    SipDialogueManager();

    SipTransaction * updateTransaction(SipMessage *sipMessage);

    SipDialogue * retrieveDialogue(std::string callId);
    SipDialogue * retrieveDialogue(uint32_t channelId);

private:

    SipTransaction * startTransaction(SipRequest * sipRequest);

    SipTransaction * getTransaction(uint32_t channelId);
    void endTransaction(uint32_t channelId);
    void releaseDialogue(uint32_t channelId);

private:
    uint32_t getNextChannelId();
    std::map<uint32_t, SipDialogue *> channelToDialogueMap;
    std::map<std::string, SipDialogue *> callIdToDialogueMap;

private:
    uint32_t lastChannelId = 0;

};


#endif //_SESSIONMANAGER_H
