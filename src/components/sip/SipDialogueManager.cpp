//
// Created by Simon Willis on 03/06/2016.
//

#include "SipDialogueManager.h"
#include "messages/SipResponse.h"
#include "messages/SipInviteRequest.h"

using namespace std;

SipDialogueManager::SipDialogueManager() {
    channelToDialogueMap.clear();
    callIdToDialogueMap.clear();
}

SipTransaction * SipDialogueManager::startTransaction(SipRequest * sipRequest) {
    if (! sipRequest) {
        fprintf(stderr, "Dialogue Manager cannot start transcation with empty request\n");
        return nullptr;
    }
    std::string callId = sipRequest->getCallId();
    fprintf(stderr, "SipDialogueManager::startTransaction [callId=%s]\n", callId.c_str());

    if (callIdToDialogueMap.find(sipRequest->getCallId()) != callIdToDialogueMap.end()) {
        fprintf(stderr, "SipDialogueManager::startTransaction [callId=%s] already exists in map\n", callId.c_str());
        return 0;
    }

    SipDialogue * dialogue = retrieveDialogue(callId);

    if (dialogue) {
        dialogue->setCurrentTransaction(sipRequest);
    } else {
        uint32_t  channelId = getNextChannelId();
        fprintf(stderr, "SipDialogueManager::startTransaction, creating a new Dialogue instance [channelId=%u] [callId=%s]\n", channelId, callId.c_str());
        dialogue = new SipDialogue(channelId, sipRequest);
        channelToDialogueMap.insert(std::make_pair(channelId, dialogue));
        callIdToDialogueMap.insert(std::make_pair(callId, dialogue));
    }

    fprintf(stderr, "startTransaction [channelId=%u]\n", dialogue->getChannelId());

    return dialogue->getCurrentTransaction();
}

SipDialogue * SipDialogueManager::retrieveDialogue(uint32_t channelId) {
    SipDialogue * session = nullptr;

    fprintf(stderr, "SipDialogueManager::retrieveDialogue [channelId=%u]\n", channelId);

    std::map<uint32_t, SipDialogue *>::iterator pos = channelToDialogueMap.find(channelId);

    if (pos != channelToDialogueMap.end()) {
        session = (*pos).second;
    } else {
        fprintf(stderr, "SipDialogueManager::retrieveDialogue Failed to retrieve dialogue for call [channelId=%d]\n", channelId);
    }

    return session;
}

SipDialogue * SipDialogueManager::retrieveDialogue(string callId) {
    SipDialogue * session = nullptr;

    fprintf(stderr, "SipDialogueManager::retrieveDialogue [callId=%s]\n", callId.c_str());

    std::map<std::string, SipDialogue *>::iterator it = callIdToDialogueMap.find(callId);

    if (it != callIdToDialogueMap.end()) {
        session = (*it).second;
    } else {
        fprintf(stderr, "SipDialogueManager::retrieveDialogue Failed to retrieve dialogue for call [callId=%s]\n", callId.c_str());
    }

    return session;
}


SipTransaction * SipDialogueManager::getTransaction(uint32_t channelId) {
    fprintf(stderr, "SipDialogueManager::getTransaction(channelId=%u)\n", channelId);
    SipTransaction * transaction = nullptr;
    SipDialogue * dialogue = retrieveDialogue(channelId);
    if (dialogue) {
        transaction = dialogue->getCurrentTransaction();
    }
    return transaction;
}

void SipDialogueManager::endTransaction(uint32_t channelId) {
    fprintf(stderr, "SipDialogueManager::endTransaction(channelId=%u)\n", channelId);
    SipDialogue * dialogue = retrieveDialogue(channelId);
    if (dialogue) {
        dialogue->endCurrentTransaction();
    }
}

void SipDialogueManager::releaseDialogue(uint32_t channelId) {
    fprintf(stderr, "SipDialogueManager::releaseDialogue [channelId=%u]\n", channelId);
    //TODO Implement SipDialogueManager::releaseDialogue
    fprintf(stderr, "SipDialogueManager::releaseDialogue [channelId=%d] Not Implemented Yet\n", channelId);
}

uint32_t SipDialogueManager::getNextChannelId() {
    //TODO Update SipDialogueManager::getNextChannelId() to use atomic variable
    uint32_t channelId = ++lastChannelId;
    fprintf(stderr, "SipDialogueManager::getNextChannelId returning channelId %u\n", channelId);
    return channelId;
}

SipTransaction *  SipDialogueManager::updateTransaction(SipMessage *sipMessage) {
    fprintf(stderr, "SipDialogueManager::updateTransaction\n");
    uint32_t channelId = 0;
    int error = 0;
    SipTransaction * transaction = nullptr;
    SipRequest * request = nullptr;
    SipResponse * response = nullptr;

    if (sipMessage == NULL) {
        fprintf(stderr, "SipDialogueManager::updateTransaction received an empty message\n");
        return transaction;
    }

    if (sipMessage->isRequest()) {
        fprintf(stderr, "SipDialogueManager::updateTransaction message is a request\n");
        request = (SipRequest *)sipMessage;
    } else {
        fprintf(stderr, "SipDialogueManager::updateTransaction Check the code for handling of responses\n");
        response = (SipResponse *)sipMessage;
    }

    if (request) {
        //TODO Sort out handling of transactions in dialogues
        if (request->getRequestType() == SipMessage::SipMessageType::INVITE) {
            SipInviteRequest * invite = (SipInviteRequest *)sipMessage;
            fprintf(stderr, "SipDialogueManager::updateTransaction have an INVITE request\n");
            SipDialogue * dialogue = retrieveDialogue(sipMessage->getCallId());
            if (dialogue) {
                fprintf(stderr, "SipDialogueManager::updateTransaction located an existing dialogue for INVITE\n");
                transaction = dialogue->getCurrentTransaction();
                if (transaction) {
                    fprintf(stderr, "SipDialogueManager::updateTransaction have active %s request [CSeq=%s]\n", transaction->sipRequest->getTypeDescriptionString(), transaction->sipRequest->getCSequence().c_str());
                    if (transaction->sipRequest->getRequestType() == SipMessage::SipMessageType::INVITE) {
                        if (invite->getCSequence().compare(transaction->sipRequest->getCSequence())) {
                            fprintf(stderr, "SipDialogueManager::updateTransaction sequences don't match, setting as a ReINVITE\n");
                            invite->isReinvite(true);
                        } else {
                            fprintf(stderr, "SipDialogueManager::updateTransaction Sequence numbers match, ignoring\n");
                        }
                    } else {
                        fprintf(stderr, "SipDialogueManager::updateTransaction assuming this INVITE is a ReINVITE\n");
                    }
                } else {
                    fprintf(stderr, "SipDialogueManager::updateTransaction no active request\n");
                }
            } else {
                fprintf(stderr, "SipDialogueManager::updateTransaction no existing dialogue for INVITE\n");
                transaction = startTransaction(request);
                fprintf(stderr, "New transaction returned as %s\n", transaction->toString().c_str());
                dialogue = retrieveDialogue(transaction->channelId);
                //TODO Associate transaction with dialogue
                if (!dialogue) {
                    fprintf(stderr, "SipDialogueManager::updateTransaction Failed to allocate a valid dialogue for INVITE\n");
                }
            }
        } else {
            fprintf(stderr, "SipDialogueManager::updateTransaction non-INVITE message\n");
            SipDialogue * dialogue = retrieveDialogue(sipMessage->getCallId());
            if (dialogue) {
                fprintf(stderr, "SipDialogueManager::updateTransaction located an existing dialogue\n");
                transaction = dialogue->getCurrentTransaction();
                if (transaction) {
                    fprintf(stderr, "SipDialogueManager::updateTransaction located an existing transaction\n");
                }
            }
        }

    } else {
        SipDialogue * dialogue = retrieveDialogue(response->getCallId());
        if (! dialogue) {
            fprintf(stderr, "SipDialogueManager::updateTransaction Failed to retrieve dialogue for %s %s message\n",
                    (sipMessage->isRequest()
                         ? ((SipRequest *) sipMessage)->getRequestMethod()
                         : SipMessage::getTypeStr(((SipResponse *)sipMessage)->getResponseType())),
                    (sipMessage->isRequest() ? "Request":"Response"));
        } else {
            channelId = dialogue->getChannelId();
            fprintf(stderr, "SipDialogueManager::updateTransaction Retrieved dialogue for channel %u\n", channelId);
            sipMessage->setChannelId(channelId);
            if (sipMessage->isRequest()) {
                if (((SipRequest *)sipMessage)->getRequestType() != SipMessage::SipMessageType::ACK) {
                    dialogue->setCurrentTransaction((SipRequest *) sipMessage);
                }
            }
        }
    }

    fprintf(stderr, "SipDialogueManager::updateTransaction: completing\n");

    return transaction;
}
