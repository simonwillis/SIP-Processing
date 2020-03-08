/// @file Session.h
/// @brief Class for holding and minipulating session data. Session data relates to the whole of the call over
/// multiple dialogues.
/// @author Simon Willis
/// @bug

#ifndef SIP_SESSION_H
#define SIP_SESSION_H


#include <string>
#include <list>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include "messages/SipMessage.h"
#include "messages/SipRequest.h"
#include "SipTransaction.h"

class SipDialogue {

public:

    /// @brief A dialogue must be in one and only one dialogue state.
    enum class DialogueState {
        NONE = 0x00,
        INCOMING,
        OUTGOING,
        CONNECTED,
        CLEARING,
        CLEARED
    };

public:

    SipDialogue(uint32_t channelId, SipRequest * sipRequest);

    SipMessage::SipMessageTransportType getTransportType() { return transportType; }

    std::string getResponseTag() { return responseTag; }
    uint32_t getChannelId() { return channelId; };
    std::string getCallId() { return callId; }

    void setCurrentTransaction(SipRequest *request);
    SipTransaction * getCurrentTransaction() { return currentTransaction; }
    void endCurrentTransaction() {
        //TODO backup dialogue specific data on transaction completion
        //TODO delete all transaction data on transaction completion
        delete currentTransaction;
        currentTransaction = nullptr;
    }

    void setState(DialogueState state) { dialogueState = state; }
    DialogueState getState() { return dialogueState; }
    const char * getStateStr();

    void setRecording(bool state) { recordingState = state; }
    bool isRecording() { return recordingState; }

private:
    const std::string callId;
    const uint32_t channelId;
    std::vector<SipMessage *> messages = std::vector<SipMessage *>();
    std::vector<char *> trace = std::vector<char *>();
    SipTransaction * currentTransaction = nullptr;
    std::string responseTag;
    DialogueState dialogueState = DialogueState::NONE;
    bool recordingState = false;
    SipMessage::SipMessageTransportType transportType = SipMessage::SipMessageTransportType::UDP;

    std::shared_ptr<spdlog::logger> logger;

    //TODO Handle specific dialogue Ids for controller requests and notifications, i.e. recording start AND stop need the requests original ID
};


#endif //SIP_SESSION_H
