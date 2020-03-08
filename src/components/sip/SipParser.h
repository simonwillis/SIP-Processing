//
// Created by Simon Willis on 03/06/2016.
//

#ifndef _SIPPARSER_H
#define _SIPPARSER_H

#include <string>
#include <map>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include "messages/SipMessage.h"


// forward declaration to allow us to pass the connection back with the callback
class SipConnection;

typedef void (* SipMessageEventHandler)(SipMessage *, void *);

class SipParser {

public:
    SipParser(SipMessageEventHandler eventHandler, void * callbackData = nullptr);

    size_t loadMessageData(const char * buf, size_t len);

    static const std::vector<char> messageDelimiter;

private:

    bool processingMessage = false;
    SipMessage * parseHeaderData(const char * message, const size_t messageSize);
    void parseBodyData(const char * message, const size_t messageSize);

private:

    static bool isValidHeaderFieldNameChar(int c);
    static size_t readHeaderLine(size_t &pos, const char *message, const size_t messageSize, char *line, size_t sizeOfLine,
                          size_t &lineLength, char *method, size_t sizeOfMethod, size_t &methodLength,
                          bool &endOfHeaders, bool &endOfMessage, int &error);

    void normaliseHeaderData(const char * message, const size_t messageSize);

private:

    normalised_sip_message_data_t data;
    int requiredContentLength = 0;
    int receivedContentLength = 0;
    std::string bodyBoundaryKey = "uniqueBoundary";
    SipMessageBody::BodyType bodyType = SipMessageBody::BodyType::NONE;
    SipMessageBody::BodyType expectingBodyContentType;
    std::string expectingBodyContentTypeString;


    SipMessage * sipMessage = NULL;
    SipMessageEventHandler eventHandler;
    void * callbackData = NULL;

protected:
    std::shared_ptr<spdlog::logger> logger;


};

#endif //_SIPPARSER_H
