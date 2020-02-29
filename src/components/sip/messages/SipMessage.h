/// @file SipMessage.h
/// @brief Base virtual class on which all SIP Requests and Responses are built.
/// @author Simon Willis
/// @bug

#ifndef CORE_SIPEVENT_H
#define CORE_SIPEVENT_H

#include <string>
#include <list>
#include <map>
#include "../headers/SipHeader.h"
#include "SipMessageBody.h"

// forward declaration of Session
class SipDialogue;

typedef struct normalised_sip_message_data {
    std::string message;
    //std::string sdp;
    //std::string xml;
    std::map<SipMessageBody::BodyType , std::string> bodies;
    //std::string raw;
} normalised_sip_message_data_t;


class SipMessage {

public:

    static const size_t MaxMethodLength = 64;
    static const size_t MaxHeaderLineLength = 512;

    enum class SipMessageType {
        UNKNOWN   = 0x0000,   // Default response when cannot be interpreted
        INVITE    = 0x0001,	  // RFC 3261 - Indicates a client is being invited to participate in a call session.
        ACK	      = 0x0002,   // RFC 3261 - Confirms that the client has received a final response to an INVITE request.
        BYE	      = 0x0003,   // RFC 3261 - Terminates a call and can be sent by either the caller or the callee.
        CANCEL	  = 0x0004,   // RFC 3261 - Cancels any pending request.
        OPTIONS	  = 0x0005,   // RFC 3261 - Queries the capabilities of servers.
        REGISTER  = 0x0006,   // RFC 3261 - Registers the address listed in the To header field with a SIP server.
        PRACK     = 0x0007,   // RFC 3262 - Provisional acknowledgement.
        SUBSCRIBE = 0x0008,   // RFC 6665 - Subscribes for an Event of Notification from the Notifier.
        NOTIFY    = 0x0009,   // RFC 6665 - Notify the subscriber of a new Event.
        PUBLISH   = 0x000A,   // RFC 3903 - Publishes an event to the Server.
        INFO      = 0x000B,   // RFC 6086 - Sends mid-session information that does not modify the session state.
        REFER     = 0x000C,   // RFC 3515 - Asks recipient to issue SIP request (call transfer.)
        MESSAGE   = 0x000D,   // RFC 3428 - Transports instant messages using SIP.
        UPDATE    = 0x000E,   // RFC 3311 - Modifies the state of a session without changing the state of the dialog.
        TRYING    = 0x0101,
        PROGRESS  = 0x0102,
        RINGING   = 0x0103,
        OK        = 0x0104,
        SERVICE_UNAVAILABLE = 0x0105,
    };

    static const char * getTypeStr(SipMessageType type) {
        switch (type) {
            case SipMessageType::INVITE: return "INVITE";
            case SipMessageType::ACK: return "ACK";
            case SipMessageType::BYE: return "BYE";
            case SipMessageType::OPTIONS: return "OPTIONS";
            case SipMessageType::CANCEL: return "CANCEL";
            case SipMessageType::REGISTER: return "REGISTER";
            case SipMessageType::PRACK: return "PRACK";
            case SipMessageType::SUBSCRIBE: return "SUBSCRIBE";
            case SipMessageType::NOTIFY: return "NOTIFY";
            case SipMessageType::PUBLISH: return "PUBLISH";
            case SipMessageType::INFO: return "INFO";
            case SipMessageType::REFER: return "REFER";
            case SipMessageType::MESSAGE: return "MESSAGE";
            case SipMessageType::UPDATE: return "UPDATE";
            case SipMessageType::TRYING: return "PROGRESS";
            case SipMessageType::PROGRESS: return "PROGRESS";
            case SipMessageType::RINGING: return "RINGING";
            case SipMessageType::OK: return "OK";
            case SipMessageType::SERVICE_UNAVAILABLE: return "SERVICE_UNAVAILABLE";

            case SipMessageType::UNKNOWN:
            default: return "UNKNOWN";
        }
    }

    enum class SipMessageTransportType {
        UNKNOWN   = 0x00,
        UDP       = 0x01,
        TCP       = 0x02
    };

    static const SipMessageTransportType DefaultTransportType = SipMessageTransportType::UDP;

    static SipMessage * builder(normalised_sip_message_data_t data);

    virtual const char * getTypeDescriptionString() = 0;

    virtual const char * getSubTypeDescriptionString() = 0;

public:

    /// @brief get flag indication of support for the message type. Some message types are modelled but not supported
    /// @return true if the message type is functionally supported
    virtual const bool supported() = 0;

    /// @brief check if this is a Response type message
    /// @return true if this is a response message
    virtual const bool isResponse() = 0;

    /// @brief check if this is a Request type message
    /// @return true if this is a request message
    virtual const bool isRequest() = 0;

    virtual const std::string getMessageBody();

    SipMessageBody::BodyType getContentType();

    void addHeader(SipHeader * sipHeader);

    /// @brief add a body to the message, if a different type already exists this becomes a message with a
    /// multi-body, if the same type already exists the original is replaced
    void addBody(SipMessageBody::BodyType type, std::string body);

    /// @brief checks if the message has a body
    /// @return true if has body content
    bool hasBody();

    /// @brief checks for contained bodies
    /// @returns returns vector containing list of all body types contained in the message
    std::vector<SipMessageBody::BodyType> getBodyTypes();

    /// @brief checks if the message has a body of the specified type, multi-bodies will be checked for
    /// body types contained within
    /// @return true if the message contains a body of the specified type
    bool hasBody(SipMessageBody::BodyType type);

    /// @brief get the body of the specified type from the message
    /// @return a body entry or an empty string
    std::string getBody(SipMessageBody::BodyType type);

    /// @brief get all the bodies associated with the message
    /// @return a set of bodies as a map of BodyType to string
    std::map<SipMessageBody::BodyType, std::string> getBodies();


protected:

    SipMessage();
    SipMessage(SipMessage::SipMessageType method, const char * via,
               const char to, const char * from, const char * callId, const char * contact, uint32_t seqId);
    SipMessage(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers, std::map<SipMessageBody::BodyType, std::string> bodies);
    mapped_buffer_t allocateBuffer(const char * data, size_t size);

    const char * UserAgentId = "Core-0-1\0";

public:
    //static size_t readHeaderLine(size_t &pos, const char * message, const size_t messageSize, char * line, size_t lineSize, size_t &lineLength, char * method, size_t sizeOfMethod, size_t &methodLength, bool &endOfHeaders, bool &endOfMessage, int &error);

    static const char * getTransportTypeStr(SipMessageTransportType transportType) {
        switch (transportType) {
            case SipMessageTransportType::UDP: return "UDP";
            case SipMessageTransportType::TCP: return "TCP";
            default: return "UDP";
        }
    }

    const char * getTransportTypeStr() {
        return getTransportTypeStr(transportType);
    }

    std::string getCallId();
    std::string getFrom();
    std::string getTo();
    std::string getRemoteAddress();
    std::string getResponseViaContent();
    int getContentLength();

    int getRemotePort();
    SipMessage::SipMessageTransportType getTransportType();

    SipMessage::SipMessageTransportType transportType = DefaultTransportType;
    std::string getCSequence();

    const char * message = NULL;

    uint32_t getChannelId() { return channelId; }
    void setChannelId(int32_t channelId) { this->channelId = channelId; }

    sip_headers_t getHeaders();

//    void setSession(SipDialogue * session) { this->session = session; }
//    SipDialogue * getSession() { return this->session; }


//    uint32_t getConnectionId() { return this->connectionId; };
//    void setConnectionId(uint32_t connectionId) { this->connectionId = connectionId; }


private:
    static bool isValidHeaderFieldNameChar(int c);

    mapped_buffer_t propertiesLine;
    std::map<std::string, SipHeader *> headers;
    std::map<SipMessageBody::BodyType, std::string> bodies;

    uint32_t channelId;
//    size_t contentLength = 0;
//    SipMessageBody::BodyType contentType = SipMessageBody::BodyType::NONE;

//    /// @brief back-link to the Session that this message belongs to
//    SipDialogue * session;

    char messageMemory[4096];
    size_t messageMemoryOffset = 0;

};


#endif //CORE_SIPEVENT_H
