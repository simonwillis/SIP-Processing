//
// Created by Simon Willis on 03/06/2016.
//

#include <sstream>
#include <vector>
#include "SipParser.h"
#include "../utilities/util.hpp"

using namespace std;


#define DEBUG_SIPPARSER false

const std::vector<char> SipParser::messageDelimiter = std::vector<char>{'\r', '\n', '\r', '\n'};

SipParser::SipParser(SipMessageEventHandler eventHandler, void * callbackData)
        : eventHandler(eventHandler), callbackData(callbackData) {
    logger = spdlog::get("stdlogger");
}

size_t SipParser::loadMessageData(const char * buf, size_t len) {

    for (size_t i = 0; i < len; ++i) {
        printChar(buf[i]);
    }

    logger->debug("SipParser::loadMessageData passed {} bytes, {} sipMessage", len, (sipMessage ? "already have a": "no existing"));

    if (!processingMessage) {
        logger->debug("SipParser::loadMessageData Waiting for message, received {} bytes", len);
        this->sipMessage = parseHeaderData(buf, len);
        if (sipMessage) {
            if (sipMessage->getContentLength() == 0) {
                logger->debug("SipParser::loadMessageData calling event handler with completed SipMessage, content length is zero");
                this->eventHandler(sipMessage, this->callbackData);
                receivedContentLength = 0;
                processingMessage = false;
            } else {
                logger->debug("SipParser::loadMessageData {} {} content length = {}", sipMessage->getSubTypeDescriptionString(), sipMessage->getTypeDescriptionString(), sipMessage->getContentLength());
                bodyType = sipMessage->getContentType();

                logger->debug("SipParser::loadMessageData body type = {} [expecting={}]", SipMessageBody::toString(sipMessage->getContentType()), SipMessageBody::toString(this->expectingBodyContentType));
                expectingBodyContentType = (bodyType == SipMessageBody::BodyType::MULTIPLE) ? SipMessageBody::BodyType::BOUNDARY : bodyType;
                logger->debug("SipParser::loadMessageData now expecting body type = {} [expecting={}]\n", SipMessageBody::toString(sipMessage->getContentType()), SipMessageBody::toString(this->expectingBodyContentType));
                processingMessage = true;
            }
            return sipMessage->getContentLength();
        } else {
            return 0;
        }
    }

    logger->warn("SipParser::loadMessageData body type = {} [expecting={}]", SipMessageBody::toString(sipMessage->getContentType()), SipMessageBody::toString(this->expectingBodyContentType));

    receivedContentLength += len;

    parseBodyData(buf, len);

    if (receivedContentLength >= sipMessage->getContentLength()) {
        logger->debug("SipParser::loadMessageData: Received content length = {}, Content-Length = {} - calling eventHandler", receivedContentLength, sipMessage->getContentLength());
        this->eventHandler(sipMessage, this->callbackData);
        receivedContentLength = 0;
        processingMessage = false;
        return 0;
    } else {
        logger->debug("SipParser::loadMessageData: Received content length = {}, Content-Length = {} - waiting for more data", receivedContentLength, sipMessage->getContentLength());
        processingMessage = true;
        return sipMessage->getContentLength() - receivedContentLength;
    }

}


SipMessage * SipParser::parseHeaderData(const char * message, const size_t messageSize) {

    SipMessage * msg;
    normaliseHeaderData(message, messageSize);
    msg = SipMessage::builder(data);

    if (msg) {
        requiredContentLength = msg->getContentLength();
        if (requiredContentLength) {
            expectingBodyContentType = (msg->getContentType() == SipMessageBody::BodyType::MULTIPLE)
                                       ? SipMessageBody::BodyType::BOUNDARY
                                       : msg->getContentType();
        } else {
            logger->debug("SipParser::parseHeaderData content length is zero so setting expected body type to NONE");
            expectingBodyContentType = SipMessageBody::BodyType::NONE;
        }

        logger->debug("SipParser::parseHeaderData Picked up new SIP Message. Required Content Length = {}, expecting body type {}", requiredContentLength, SipMessageBody::toString(expectingBodyContentType));
    } else {
        logger->warn("SipParser::parseHeaderData call to builder returned a NULL sipMessage");
    }
    return msg;
}

void SipParser::parseBodyData(const char *message, const size_t messageSize) {

    logger->debug("SipParser::parseBodyData [contentType={}] [expectingBodyContentType={}]", SipMessageBody::toString(bodyType), SipMessageBody::toString(expectingBodyContentType));

    mapped_buffer_t msgBuffer = mapped_buffer(message, messageSize);

    SipMessageBody::BodyType nextBodyType = SipMessageBody::BodyType::NONE;

    switch (expectingBodyContentType) {
        case SipMessageBody::BodyType::BOUNDARY: {
            logger->debug("SipParser::parseBodyData Processing body data type as BOUNDARY");

            vector<mapped_buffer_t> lines = buffer_split_lines(msgBuffer);

            logger->debug("SipParser::parseBodyData Body has {} lines", lines.size());

            if (buffer_contains(lines[0], bodyBoundaryKey.c_str())) {

                if (buffer_ends_with(lines[0], "--")) {
                    // the is the close off of the data set we should not expect any further data

                    logger->debug("SipParser::parseBodyData Located boundary entry for end of multipart data");

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
                                logger->warn("SipParser::parseBodyData Don't know how to handle content type {} storing as OTHER", string(lines[lineIndex].buf, lines[lineIndex].len));
                                nextBodyType = SipMessageBody::BodyType::OTHER;
                            }
                        }
                    }
                }
            }
        }
        break;

        case SipMessageBody::BodyType::SDP:
            logger->debug("SipParser::parseBodyData Processing body data type as SDP");

            data.bodies.insert(std::make_pair(SipMessageBody::BodyType::SDP, string(message, messageSize)));
            sipMessage->addBody(SipMessageBody::BodyType::SDP, bufferToString(message));

            nextBodyType = (bodyType == SipMessageBody::BodyType::MULTIPLE) ? SipMessageBody::BodyType::BOUNDARY : SipMessageBody::BodyType::NONE;
            break;
        case SipMessageBody::BodyType::XML:
            logger->debug("SipParser::parseBodyData Processing body data type as XML");

            data.bodies.insert(std::make_pair(SipMessageBody::BodyType::XML, string(message, messageSize)));
            sipMessage->addBody(SipMessageBody::BodyType::XML, bufferToString(message));
            nextBodyType = (bodyType == SipMessageBody::BodyType::MULTIPLE) ? SipMessageBody::BodyType::BOUNDARY : SipMessageBody::BodyType::NONE;
            break;
        case SipMessageBody::BodyType::OTHER:
            logger->debug("SipParser::parseBodyData Processing body data type as OTHER []", string(message, messageSize));

            data.bodies.insert(std::make_pair(SipMessageBody::BodyType::OTHER, string(message, messageSize)));
            nextBodyType = (bodyType == SipMessageBody::BodyType::MULTIPLE) ? SipMessageBody::BodyType::BOUNDARY : SipMessageBody::BodyType::NONE;
            break;
        case SipMessageBody::BodyType::NONE:
            logger->debug("SipParser::parseBodyData Attempting to parse body data but the expected datatype is NONE");
            break;
        case SipMessageBody::BodyType::MULTIPLE:
            logger->debug("SipParser::parseBodyData Unexxpectedly waiting for MULTIPLE body type when this case should have already been processed");
            nextBodyType = SipMessageBody::BodyType::NONE;
            break;
        case SipMessageBody::BodyType::UNKNOWN:
            logger->warn("SipParser::parseBodyData Unexxpectedly waiting for UNKNOWN body type");
            nextBodyType = SipMessageBody::BodyType::NONE;
            break;
    }

    // Reset the body Type so that the routine knows to parse the next incoming descriptor block
    expectingBodyContentType = nextBodyType;

    logger->debug("SipParser::parseBodyData set [expectingBodyContentType={}]", SipMessageBody::toString(expectingBodyContentType));

}


void SipParser::normaliseHeaderData(const char * message, const size_t messageSize) {

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

    // Cover off the case that we have got to the end of the message
    if (pos >= messageSize) {
        endOfMessage = true;
        return 0;
    }

    for (messageIndex = pos; (pMessage < (message + messageSize) && (pLine < (line + sizeOfLine - 1)));
            ++messageIndex, ++pMessage, ++pos) {
        if (! haveName) {
            if ((methodLength >= sizeOfMethod - 1)|| (! isValidHeaderFieldNameChar(*pMessage))) {
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

    return lineLength;
}