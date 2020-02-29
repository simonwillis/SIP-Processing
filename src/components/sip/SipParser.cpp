//
// Created by Simon Willis on 03/06/2016.
//

#include <sstream>
#include <vector>
#include "SipParser.h"
#include "../utilities/util.hpp"
#include "../../../tools/RaiiLog.h"

using namespace std;


#define DEBUG_SIPPARSER false

const std::vector<char> SipParser::messageDelimiter = std::vector<char>{'\r', '\n', '\r', '\n'};

SipParser::SipParser(SipMessageEventHandler eventHandler, void * callbackData)
        : eventHandler(eventHandler), callbackData(callbackData) {
    //fprintf(stderr, "SipParser::CTOR\n");
}

size_t SipParser::loadMessageData(const char * buf, size_t len) {
    RaiiLog raiiLog = RaiiLog("SipParser::loadMessageData");

    if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::loadMessageData\n");
    for (size_t i = 0; i < len; ++i) {
        printChar(buf[i]);
    }

    if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::loadMessageData passed %zu bytes, %s sipMessage\n", len, (sipMessage ? "already have a": "no existing"));

    if (!processingMessage) {
        if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::loadMessageData Waiting for message, received %ld bytes\n", len);
        this->sipMessage = parseHeaderData(buf, len);
        if (sipMessage) {
            if (sipMessage->getContentLength() == 0) {
                if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::loadMessageData calling event handler with completed SipMessage, content length is zero\n");
                this->eventHandler(sipMessage, this->callbackData);
                //fprintf(stderr, "SipParser::loadMessageData purging local copy of SipMessage\n");
                receivedContentLength = 0;
                processingMessage = false;
            } else {
                if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::loadMessageData (%s %s) content length = %d\n", sipMessage->getSubTypeDescriptionString(), sipMessage->getTypeDescriptionString(), sipMessage->getContentLength());
                bodyType = sipMessage->getContentType();

                if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::loadMessageData body type = %s [expecting=%s]\n", SipMessageBody::toString(sipMessage->getContentType()), SipMessageBody::toString(this->expectingBodyContentType));
                expectingBodyContentType = (bodyType == SipMessageBody::BodyType::MULTIPLE) ? SipMessageBody::BodyType::BOUNDARY : bodyType;
                if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::loadMessageData now expecting body type = %s [expecting=%s]\n", SipMessageBody::toString(sipMessage->getContentType()), SipMessageBody::toString(this->expectingBodyContentType));
                processingMessage = true;
            }
            return sipMessage->getContentLength();
        } else {
            return 0;
        }

    }

    //fprintf(stderr, "SipParser::loadMessageData Have a sipMessage at address %p\n", sipMessage);
    if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::loadMessageData body type = %s [expecting=%s]\n", SipMessageBody::toString(sipMessage->getContentType()), SipMessageBody::toString(this->expectingBodyContentType));

    receivedContentLength += len;
    //fprintf(stderr, "SipParser::loadMessageData Waiting for %ld bytes of body data, received %ld, total %d\n", sipMessage->getContentLength(), len, receivedContentLength);

    parseBodyData(buf, len);

    if (receivedContentLength >= sipMessage->getContentLength()) {
        if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::loadMessageData: Received content length = %d, Content-Length = %d - calling eventHandler\n", receivedContentLength, sipMessage->getContentLength());
        this->eventHandler(sipMessage, this->callbackData);
        receivedContentLength = 0;
        processingMessage = false;
        return 0;
    } else {
        fprintf(stderr, "SipParser::loadMessageData: Received content length = %d, Content-Length = %d - waiting for more data\n", receivedContentLength, sipMessage->getContentLength());
        processingMessage = true;
        return sipMessage->getContentLength() - receivedContentLength;
    }

}


SipMessage * SipParser::parseHeaderData(const char * message, const size_t messageSize) {
    RaiiLog raiiLog = RaiiLog("SipParser::parseHeaderData");
    SipMessage * msg;
    normaliseHeaderData(message, messageSize);
    msg = SipMessage::builder(data);

    if (msg) {

        if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseHeaderData call to builder returned sipMessage at %p\n", msg);

        requiredContentLength = msg->getContentLength();
        if (requiredContentLength) {
            expectingBodyContentType = (msg->getContentType() == SipMessageBody::BodyType::MULTIPLE)
                                       ? SipMessageBody::BodyType::BOUNDARY
                                       : msg->getContentType();
        } else {
            if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseHeaderData content length is zero so setting expected body type to NONE\n");
            expectingBodyContentType = SipMessageBody::BodyType::NONE;
        }

        if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseHeaderData Picked up new SIP Message. Required Content Length = %d, expecting body type %s\n", requiredContentLength, SipMessageBody::toString(expectingBodyContentType));
    } else {
        fprintf(stderr, "SipParser::parseHeaderData call to builder returned a NULL sipMessage\n");
    }
    return msg;
}

void SipParser::parseBodyData(const char *message, const size_t messageSize) {
    RaiiLog raiiLog = RaiiLog("SipParser::parseBodyData\n");

    if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData [contentType=%s] [expectingBodyContentType=%s]\n", SipMessageBody::toString(bodyType), SipMessageBody::toString(expectingBodyContentType));

    mapped_buffer_t msgBuffer = mapped_buffer(message, messageSize);

    SipMessageBody::BodyType nextBodyType = SipMessageBody::BodyType::NONE;

    switch (expectingBodyContentType) {
        case SipMessageBody::BodyType::BOUNDARY: {
            if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData Processing body data type as BOUNDARY\n");

            vector<mapped_buffer_t> lines = buffer_split_lines(msgBuffer);

            if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData Body has %zu lines\n", lines.size());

            if (buffer_contains(lines[0], bodyBoundaryKey.c_str())) {

                if (buffer_ends_with(lines[0], "--")) {
                    // the is the close off of the data set we should not expect any further data

                    if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData Located boundary entry for end of multipart data\n");

                    nextBodyType = SipMessageBody::BodyType::NONE;

                } else {

                    // this is the entry for a boundary header, we should at least get the Content-Type of the following body
                    for (int lineIndex = 1; lineIndex < lines.size(); ++lineIndex) {
                        if (buffer_contains(lines[lineIndex], "Content-Type")) {
                            if (buffer_contains(lines[lineIndex], "sdp")) {
                                nextBodyType = SipMessageBody::BodyType::SDP;
                            } else if (buffer_contains(lines[lineIndex], "xml")) {
                                nextBodyType = SipMessageBody::BodyType::XML;
                            } else {
                                fprintf(stderr, "SipParser::parseBodyData Don't know how to handle content type '%s' storing as OTHER\n", string(lines[lineIndex].buf, lines[lineIndex].len).c_str());
                                nextBodyType = SipMessageBody::BodyType::OTHER;
                            }
                        }
                    }
                }
            }
        }
        break;

        case SipMessageBody::BodyType::SDP:
            if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData Processing body data type as SDP\n");

            data.bodies.insert(std::make_pair(SipMessageBody::BodyType::SDP, string(message, messageSize)));
            sipMessage->addBody(SipMessageBody::BodyType::SDP, bufferToString(message));
            //fprintf(stderr, "SipParser::parseBodyData [SDP=%s]\n", data.sdp.c_str());
            nextBodyType = (bodyType == SipMessageBody::BodyType::MULTIPLE) ? SipMessageBody::BodyType::BOUNDARY : SipMessageBody::BodyType::NONE;
            break;
        case SipMessageBody::BodyType::XML:
            if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData Processing body data type as XML\n");

            data.bodies.insert(std::make_pair(SipMessageBody::BodyType::XML, string(message, messageSize)));
            sipMessage->addBody(SipMessageBody::BodyType::XML, bufferToString(message));
            nextBodyType = (bodyType == SipMessageBody::BodyType::MULTIPLE) ? SipMessageBody::BodyType::BOUNDARY : SipMessageBody::BodyType::NONE;
            break;
        case SipMessageBody::BodyType::OTHER:
            if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData Processing body data type as OTHER [%s]\n", string(message, messageSize).c_str());

            data.bodies.insert(std::make_pair(SipMessageBody::BodyType::OTHER, string(message, messageSize)));
            nextBodyType = (bodyType == SipMessageBody::BodyType::MULTIPLE) ? SipMessageBody::BodyType::BOUNDARY : SipMessageBody::BodyType::NONE;
            break;
        case SipMessageBody::BodyType::NONE:
            if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData Attempting to parse body data but the expected datatype is NONE\n");
            break;
        case SipMessageBody::BodyType::MULTIPLE:
            if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData Unexxpectedly waiting for MULTIPLE body type when this case should have already been processed\n");
            nextBodyType = SipMessageBody::BodyType::NONE;
            break;
        case SipMessageBody::BodyType::UNKNOWN:
            if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData Unexxpectedly waiting for UNKNOWN body type\n");
            nextBodyType = SipMessageBody::BodyType::NONE;
            break;
    }

    // Reset the body Type so that the routine knows to parse the next incoming descriptor block
    expectingBodyContentType = nextBodyType;

    if (DEBUG_SIPPARSER) fprintf(stderr, "SipParser::parseBodyData set [expectingBodyContentType=%s]\n", SipMessageBody::toString(expectingBodyContentType));

}


void SipParser::normaliseHeaderData(const char * message, const size_t messageSize) {
    RaiiLog raiiLog = RaiiLog("SipParser::normaliseHeaderData");

    size_t pos = 0;
    char line[SipMessage::MaxHeaderLineLength];
    char method[SipMessage::MaxMethodLength];
    size_t lineLength = 0;
    size_t methodLength = 0;
    bool endOfHeaders = false;
    bool endOfMessage = false;
    int error = 0;
    bodyType = SipMessageBody::BodyType::NONE;

    std::stringstream ss;

    while (readHeaderLine(pos, message, messageSize, line, sizeof(line), lineLength,
                          method, sizeof(method), methodLength, endOfHeaders, endOfMessage, error)) {

        if (lineLength == 0) {
            ss << "\r\n";
        } else {
            if (methodLength > 1) {
                if (areEquivalent(method, "Content-Type")) {
                    if (areEquivalent(line, "application/sdp")) {
                        bodyType = SipMessageBody::BodyType::SDP;
                        expectingBodyContentType = SipMessageBody::BodyType::SDP;
                    } else if (buffer_starts_with(line, "multipart/mixed")) {
                        bodyType = SipMessageBody::BodyType::MULTIPLE;
                        expectingBodyContentType = SipMessageBody::BodyType::BOUNDARY;
                        std::vector <mapped_buffer_t> parts = buffer_split(line, ';');
                        if (parts.size() > 1) {
                            std::vector<mapped_buffer_t> subparts = buffer_split(parts[1], '=');
                            if ((subparts.size() > 1) && (buffer_equals(subparts[0], "boundary"))) {
                                //overrides the standard pre-set boundary key value
                                bodyBoundaryKey = string(subparts[1].buf, subparts[1].len);
                                //fprintf(stderr, "Setting Body Boundary Key to %s\n", bodyBoundaryKey.c_str());
                            }
                        }
                    } else if (buffer_contains(line, "XML")) {
                        bodyType = SipMessageBody::BodyType::XML;
                        expectingBodyContentType = SipMessageBody::BodyType::XML;
                    } else {
                        bodyType = SipMessageBody::BodyType::OTHER;
                        expectingBodyContentType = SipMessageBody::BodyType::OTHER;
                        expectingBodyContentTypeString = string(line, lineLength);
                    }
                }
                ss << string(method, methodLength) << ':' << string(line, lineLength) << "\r\n";
            } else {
                ss << string(method, methodLength) << string(line, lineLength) << "\r\n";
            }
        }
    }

    data.message = ss.str();

}


bool SipParser::isValidHeaderFieldNameChar(int c) {
    return (isalpha(c) || c == '-');
}

size_t SipParser::readHeaderLine(size_t &pos, const char * message, const size_t messageSize,
                                 char * line, size_t sizeOfLine, size_t &lineLength,
                                 char * method, size_t sizeOfMethod,
                                 size_t &methodLength, bool &endOfHeaders, bool &endOfMessage,
                                 int &error) {
    char * pMessage = (char *)(message + pos);
    char * pLine = line;
    char * pMethod = method;

    size_t messageIndex;

    bool haveName = false;
    bool haveStartOfValue = false;

    endOfHeaders = false;
    endOfMessage = false;

    lineLength = 0;
    methodLength = 0;

    memset(line, 0, sizeOfLine);
    memset(method, 0, sizeOfMethod);

    //fprintf(stderr, "SipMessage::readHeaderLine: passed pos %ld\n", pos);

    // Cover off the case that we have got to the end of the message
    if (pos >= messageSize) {
        endOfMessage = true;
        return 0;
    }

    for (messageIndex = pos; (pMessage < (message + messageSize) && (pLine < (line + sizeOfLine - 1)));
            ++messageIndex, ++pMessage, ++pos) {
        //printChar(*pMessage);
        if (! haveName) {
            if ((methodLength >= sizeOfMethod - 1)|| (! isValidHeaderFieldNameChar(*pMessage))) {
                //*pMethod = '\0';
                haveName = true;
            } else {
                // we want to ignore any line starting with '-'
                if (methodLength == 0 && *pMessage == '-') {
                    while (pos < messageSize) {
                        if (*pMessage == '\n') {
                            break;
                        } else {
                            ++pMessage;
                            ++pos;
                        }
                    }
                    continue;
                } else {
                    (*pMethod++) = (*pMessage);
                    ++methodLength;
                }
            }
        }

        if (*pMessage == '\r') {
            continue;

        } else if (*pMessage == '\n') {
            // if this is a \n and the next character is a space then the line continues - nasty, but true
            if (messageIndex < messageSize - 1 && (*(pMessage+1) == ' ' || *(pMessage+1) == '\t'))  {
                continue;
            } else {
                break;
            }

        } else {
            if (haveName && !haveStartOfValue) {
                if (*pMessage != ' ' && *pMessage != ':' && *pMessage != '\t') haveStartOfValue = true;
            }
            if (haveStartOfValue) {
                *(pLine++) = *pMessage;
                ++lineLength;
            }

        }
    }

    ++pos; // move passed the last character read
    endOfHeaders = (lineLength == 0);
    endOfMessage = (pos >= messageSize);

    //fprintf(stderr, "\nSipMessage::readHeaderLine:  completed [pos=%ld]\n", pos);
    return lineLength;
}