//
// Created by Simon Willis on 03/06/2016.
//

#include "SipDialogue.h"

using namespace std;

SipDialogue::SipDialogue(uint32_t channelId, SipRequest * sipRequest) :
        callId(sipRequest->getCallId()), channelId(channelId) {

    fprintf(stderr, "Created new dialogue with channelId=%u\n", channelId);

    responseTag = string("temporary-response-tag");

    setCurrentTransaction(sipRequest);
}


void SipDialogue::setCurrentTransaction(SipRequest *request) {

    if (currentTransaction != nullptr) {
        fprintf(stderr, "Cannot set transaction on dialgue when already have a transaction\n");
        fprintf(stderr, "%s\n", currentTransaction->toString().c_str());
        return;
    }
    std::string sdpOffer = std::string();
    switch (request->getRequestType()) {
        case SipRequest::SipMessageType::INVITE:
            sdpOffer = request->getBody(SipMessageBody::BodyType::SDP);
            break;
        case SipRequest::SipMessageType::BYE:
            break;
        case SipRequest::SipMessageType::CANCEL:
            break;
        case SipRequest::SipMessageType::INFO:
            break;
        case SipRequest::SipMessageType::MESSAGE:
            break;
        case SipRequest::SipMessageType::NOTIFY:
            break;
        case SipRequest::SipMessageType::OPTIONS:
            break;
        case SipRequest::SipMessageType::PRACK:
            break;
        case SipRequest::SipMessageType::REGISTER:
            break;
        case SipRequest::SipMessageType::PUBLISH:
            break;
        case SipRequest::SipMessageType::SUBSCRIBE:
            break;
        case SipRequest::SipMessageType::UPDATE:
            break;
        case SipRequest::SipMessageType::REFER:
            break;

        case SipMessage::SipMessageType::UNKNOWN:
        case SipMessage::SipMessageType::ACK:
        case SipMessage::SipMessageType::TRYING:
        case SipMessage::SipMessageType::RINGING:
        case SipMessage::SipMessageType::OK:
        case SipMessage::SipMessageType::PROGRESS:
        case SipMessage::SipMessageType::SERVICE_UNAVAILABLE:
            fprintf(stderr, "Dialogue::setCurrentTransaction cannot handle transaction with %s\n",
                    request->getTypeDescriptionString());
            break;

    }
    SipTransaction * transaction = new SipTransaction(channelId, request, sdpOffer);

    currentTransaction = transaction;
}

const char * SipDialogue::getStateStr() {
    switch (dialogueState) {
        case DialogueState::INCOMING:
            return "INCOMING\0";
        case DialogueState::OUTGOING:
            return "OUTGOING\0";
        case DialogueState::CONNECTED:
            return "CONNECTED\0";
        case DialogueState::CLEARING:
            return "CLEARING\0";
        case DialogueState::CLEARED:
            return "CLEARED\0";
            
        case DialogueState::NONE:
        default:
            return "NONE\0";
    }
}
