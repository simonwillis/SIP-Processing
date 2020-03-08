//
// Created by Simon Willis on 03/06/2016.
//

#include "SipDialogueManager.h"
#include "messages/SipResponse.h"
#include "messages/SipInviteRequest.h"

using namespace std;

SipDialogueManager::SipDialogueManager() {
    logger = spdlog::get("stdlogger");
    channelToDialogueMap.clear();
    callIdToDialogueMap.clear();
}

SipTransaction * SipDialogueManager::startTransaction(SipRequest * sipRequest) {
    if (! sipRequest) {
        logger->warn("Dialogue Manager cannot start transaction with empty request");
        return nullptr;
    }
    std::string callId = sipRequest->getCallId();
    logger->debug("SipDialogueManager::startTransaction [callId={}]", callId);

    if (callIdToDialogueMap.find(sipRequest->getCallId()) != callIdToDialogueMap.end()) {
        logger->warn("SipDialogueManager::startTransaction [callId={}] already exists in map", callId.c_str());
        return 0;
    }

    SipDialogue * dialogue = retrieveDialogue(callId);

    if (dialogue) {
        dialogue->setCurrentTransaction(sipRequest);
    } else {
        uint32_t  channelId = getNextChannelId();
        logger->debug("SipDialogueManager::startTransaction, creating a new Dialogue instance [channelId={}] [callId={}]", channelId, callId);
        dialogue = new SipDialogue(channelId, sipRequest);
        channelToDialogueMap.insert(std::make_pair(channelId, dialogue));
        callIdToDialogueMap.insert(std::make_pair(callId, dialogue));
    }

    logger->debug("startTransaction OK on channelId={}}", dialogue->getChannelId());

    return dialogue->getCurrentTransaction();
}

SipDialogue * SipDialogueManager::retrieveDialogue(uint32_t channelId) {
    SipDialogue * session = nullptr;

    logger->debug("SipDialogueManager::retrieveDialogue [channelId={}]", channelId);

    std::map<uint32_t, SipDialogue *>::iterator pos = channelToDialogueMap.find(channelId);

    if (pos != channelToDialogueMap.end()) {
        session = (*pos).second;
    } else {
        logger->error("SipDialogueManager::retrieveDialogue Failed to retrieve dialogue for call [channelId={}]", channelId);
    }

    return session;
}

SipDialogue * SipDialogueManager::retrieveDialogue(string callId) {
    SipDialogue * session = nullptr;

    logger->debug("SipDialogueManager::retrieveDialogue [callId={}]", callId);

    std::map<std::string, SipDialogue *>::iterator it = callIdToDialogueMap.find(callId);

    if (it != callIdToDialogueMap.end()) {
        session = (*it).second;
    } else {
        logger->warn("SipDialogueManager::retrieveDialogue Failed to retrieve dialogue for call [callId={}]", callId);
    }

    return session;
}


SipTransaction * SipDialogueManager::getTransaction(uint32_t channelId) {
    logger->debug("SipDialogueManager::getTransaction channelId={}", channelId);
    SipTransaction * transaction = nullptr;
    SipDialogue * dialogue = retrieveDialogue(channelId);
    if (dialogue) {
        transaction = dialogue->getCurrentTransaction();
    }
    return transaction;
}

void SipDialogueManager::endTransaction(uint32_t channelId) {
    logger->debug("SipDialogueManager::endTransaction channelId={}", channelId);
    SipDialogue * dialogue = retrieveDialogue(channelId);
    if (dialogue) {
        dialogue->endCurrentTransaction();
    }
}

void SipDialogueManager::releaseDialogue(uint32_t channelId) {
    logger->warn("SipDialogueManager::releaseDialogue channelId={} - NOT IMPLEMENTED", channelId);
    //TODO Implement SipDialogueManager::releaseDialogue
}

uint32_t SipDialogueManager::getNextChannelId() {
    //TODO Update SipDialogueManager::getNextChannelId() to use atomic variable
    uint32_t channelId = ++lastChannelId;
    logger->debug("SipDialogueManager::getNextChannelId returning channelId {}", channelId);
    return channelId;
}

SipTransaction *  SipDialogueManager::updateTransaction(SipMessage *sipMessage) {
    logger->debug("SipDialogueManager::updateTransaction");
    uint32_t channelId = 0;
    int error = 0;
    SipTransaction * transaction = nullptr;
    SipRequest * request = nullptr;
    SipResponse * response = nullptr;

    if (sipMessage == NULL) {
        logger->warn("SipDialogueManager::updateTransaction received an empty message");
        return transaction;
    }

    if (sipMessage->isRequest()) {
        logger->debug("SipDialogueManager::updateTransaction message is a request");
        request = (SipRequest *)sipMessage;
    } else {
        //TODO Check the code for handling of responses
        logger->warn("SipDialogueManager::updateTransaction Check the code for handling of responses");
        response = (SipResponse *)sipMessage;
    }

    if (request) {
        //TODO Sort out handling of transactions in dialogues
        if (request->getRequestType() == SipMessage::SipMessageType::INVITE) {
            SipInviteRequest * invite = (SipInviteRequest *)sipMessage;
            logger->debug("SipDialogueManager::updateTransaction have an INVITE request");
            SipDialogue * dialogue = retrieveDialogue(sipMessage->getCallId());
            if (dialogue) {
                logger->debug("SipDialogueManager::updateTransaction located an existing dialogue for INVITE");
                transaction = dialogue->getCurrentTransaction();
                if (transaction) {
                    logger->debug("SipDialogueManager::updateTransaction have active {} request [CSeq={}]", transaction->sipRequest->getTypeDescriptionString(), transaction->sipRequest->getCSequence());
                    if (transaction->sipRequest->getRequestType() == SipMessage::SipMessageType::INVITE) {
                        if (invite->getCSequence().compare(transaction->sipRequest->getCSequence())) {
                            logger->debug("SipDialogueManager::updateTransaction sequences don't match, setting as a ReINVITE");
                            invite->isReinvite(true);
                        } else {
                            logger->debug("SipDialogueManager::updateTransaction Sequence numbers match, ignoring");
                        }
                    } else {
                        logger->debug("SipDialogueManager::updateTransaction assuming this INVITE is a ReINVITE");
                    }
                } else {
                    logger->warn("SipDialogueManager::updateTransaction no active request for INVITE for existing dialogue");
                }
            } else {
                logger->warn("SipDialogueManager::updateTransaction no existing dialogue for INVITE");
                transaction = startTransaction(request);
                logger->debug("New transaction returned as {}", transaction->toString());
                dialogue = retrieveDialogue(transaction->channelId);
                //TODO Associate transaction with dialogue
                if (!dialogue) {
                    logger->error("SipDialogueManager::updateTransaction Failed to allocate a valid dialogue for INVITE");
                }
            }
        } else {
            logger->debug("SipDialogueManager::updateTransaction non-INVITE message");
            SipDialogue * dialogue = retrieveDialogue(sipMessage->getCallId());
            if (dialogue) {
                logger->debug( "SipDialogueManager::updateTransaction located an existing dialogue, callId={}", dialogue->getCallId());
                transaction = dialogue->getCurrentTransaction();
                if (transaction) {
                    logger->debug("SipDialogueManager::updateTransaction located an existing transaction on dialogue, callId={} on channel {}", dialogue->getCallId(), transaction->channelId);
                }
            }
        }

    } else {
        SipDialogue * dialogue = retrieveDialogue(response->getCallId());
        if (! dialogue) {
            logger->warn("SipDialogueManager::updateTransaction Failed to retrieve dialogue for {}, {} message",
                    (sipMessage->isRequest()
                         ? ((SipRequest *) sipMessage)->getRequestMethod()
                         : SipMessage::getTypeStr(((SipResponse *)sipMessage)->getResponseType())),
                    (sipMessage->isRequest() ? "Request":"Response"));
        } else {
            channelId = dialogue->getChannelId();
            logger->debug("SipDialogueManager::updateTransaction Retrieved dialogue for channel {}", channelId);
            sipMessage->setChannelId(channelId);
            if (sipMessage->isRequest()) {
                if (((SipRequest *)sipMessage)->getRequestType() != SipMessage::SipMessageType::ACK) {
                    dialogue->setCurrentTransaction((SipRequest *) sipMessage);
                }
            }
        }
    }

    logger->debug("SipDialogueManager::updateTransaction: completing");

    return transaction;
}
