//
// Created by Simon Willis on 23/05/2016.
//

#include <cstdlib>
#include <map>
#include "SipMessage.h"
#include "SipInviteRequest.h"
#include "../headers/SipViaHeader.h"
#include "../headers/SipFromHeader.h"
#include "../headers/SipToHeader.h"
#include "../headers/SipCallIdHeader.h"
#include "../headers/SipCSeqHeader.h"
#include "../headers/SipContentTypeHeader.h"
#include "../headers/SipContentLengthHeader.h"
#include "../headers/SipExtendedHeader.h"
#include "SipAckRequest.h"
#include "SipByeRequest.h"
#include "SipOptionsRequest.h"
#include "SipSubscribeRequest.h"
#include "../headers/SipContactHeader.h"
#include "../headers/SipMaxForwardsHeader.h"
#include "../headers/SipSubjectHeader.h"



using namespace std;

#define DEBUG_SIPMESSAGE false

SipMessage::SipMessage() {

    logger = spdlog::get("stdlogger");

}

SipMessage::SipMessage(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers, std::map<SipMessageBody::BodyType, std::string> bodies)
        : propertiesLine(propertiesLine), headers(headers), bodies(bodies) {

    logger->debug("SipMessage::CTOR [properties={}] [headercount=%{}]", propertiesLine.toString().c_str(), headers.size());

}

mapped_buffer_t SipMessage::allocateBuffer(const char * data, size_t size) {
    mapped_buffer_t buffer;
    if ((messageMemoryOffset + size) < sizeof(messageMemory)) {
        buffer = mapped_buffer(messageMemory + messageMemoryOffset, size);
        messageMemoryOffset += size;
    } else {
        logger->error( "SipMessage out of memory (update allocator to create more");
    }
    return buffer;
}

const std::string SipMessage::getMessageBody() {
    return std::string();
}

SipMessage * SipMessage::builder(normalised_sip_message_data_t normalisedData) {

    SipMessage * sipMessage = NULL;

    auto logger = spdlog::get("stdlogger");

    int contentLength;
    SipMessage::SipMessageTransportType transportType;

    SipMessageBody::BodyType contentType = SipMessageBody::BodyType::NONE;
    char * messageBuffer = (char *) malloc(normalisedData.message.size());
    memcpy(messageBuffer, normalisedData.message.c_str(), normalisedData.message.size());

    mapped_buffer_t message = mapped_buffer_t(messageBuffer, normalisedData.message.size());

    vector<mapped_buffer_t> lines = buffer_split_lines(message);
    vector<mapped_buffer_t> lineItems;
    mapped_buffer_t method;
    mapped_buffer_t methodProperties;
    mapped_buffer_t header;
    vector<mapped_buffer_t> headerItems;
    mapped_buffer_t headerFieldName;
    mapped_buffer_t headerProperties;

    map<string, SipHeader *> headers;

    if (lines.size() < 1) {
        logger->debug("SipMessage::builder: Unable to process empty message (no message lines)");
        return sipMessage;
    }

    // The first line of the message will either be a response starting with
    lineItems = buffer_split_on_first(lines[0], ':');
    method = lineItems[0];
    methodProperties = lineItems.size() > 1 ? lineItems[1] : mapped_buffer_t();

    for (size_t lineIndex = 1; lineIndex < lines.size(); ++ lineIndex) {
        headerItems = buffer_split_on_first(lines[lineIndex], ':');
        if (headerItems.size() == 1) {
            logger->warn("SipMessage::builder: [{}] header line [{}] is invalid, expected a ':' in the line", method.toString(), lineItems[lineIndex].toString());
            continue;
        }

        if (buffer_equals(headerItems[0], "VIA")) {
            SipViaHeader * header = new SipViaHeader(headerItems[1]);
            transportType = header->getTransportType();
            headers[string("VIA")] = header;

        } else if (buffer_equals(headerItems[0], "FROM")) {
            headers[string("FROM")] = new SipFromHeader(headerItems[1]);
        } else if (buffer_equals(headerItems[0], "TO")) {
            headers[string("TO")] = new SipToHeader(headerItems[1]);
        } else if (buffer_equals(headerItems[0], "CALL-ID")) {
            headers[string("CALL-ID")] = new SipCallIdHeader(headerItems[1]);
        } else if (buffer_equals(headerItems[0], "CSEQ")) {
            headers[string("CSEQ")] = new SipCSeqHeader(headerItems[1]);
        } else if (buffer_equals(headerItems[0], "CONTENT-LENGTH")) {
            SipContentLengthHeader * header = new SipContentLengthHeader(headerItems[1]);
            headers[string("CONTENT-LENGTH")] = header;

        } else if (buffer_equals(headerItems[0], "CONTENT-TYPE")) {
            headers[string("CONTENT-TYPE")] = new SipContentTypeHeader(headerItems[1]);
            if (buffer_contains(headerItems[1], "sdp")) {
                contentType = SipMessageBody::BodyType::SDP;
            } else if (buffer_contains(headerItems[1], "xml")) {
                contentType = SipMessageBody::BodyType::XML;
            } else if (buffer_contains(headerItems[1], "multipart/mixed")) {
                contentType = SipMessageBody::BodyType::MULTIPLE;
            } else {
                logger->warn("SipMessage::builder not setting content Type for {}\n", string(headerItems[1].buf, headerItems[1].len));
            }
        } else if (buffer_equals(headerItems[0], "CONTACT")) {
            headers[string("CONTACT")] = new SipContactHeader(headerItems[1]);
        } else if (buffer_equals(headerItems[0], "MAX-FORWARDS")) {
            headers[string("MAX-FORWARDS")] = new SipMaxForwardsHeader(headerItems[1]);
        } else if (buffer_equals(headerItems[0], "SUBJECT")) {
            headers[string("SUBJECT")] = new SipSubjectHeader(headerItems[1]);
        } else {
            //TODO Add all the extra header types that we are likely to expect
            headers[headerItems[0].toString().c_str()] = new SipExtendedHeader(headerItems[0], headerItems[1]);
        }
    }

    if (buffer_equals(method, "SIP")) {
        lineItems = buffer_split(lineItems[1], ' ');
        // Expecting <Version> <Code> <Text>, i.e. "2.0 100 Trying" We don't really care about the message
        if (lineItems.size() >= 2) {
            int responseCode = atoi(lineItems[1].toString().c_str());
            switch (responseCode) {
                case 100: // Trying
                    logger->warn("SipMessage::builder SIP Trying Response. Not Implemented Yet");
                    break;
                case 180: // Ringing
                    logger->warn( "SipMessage::builder SIP Ringing Response. Not Implemented Yet");
                    break;
                case 200: // Accepted
                    logger->warn("SipMessage::builder SIP Accepted Response. Not Implemented Yet");
                    break;
                case 400: // Bad Request
                    logger->warn("SipMessage::builder SIP Bad Request Response. Not Implemented Yet");
                    break;
                case 401: // Unauthorised
                    logger->warn("SipMessage::builder SIP Unauthorized Response. Not Implemented Yet");
                    break;
                case 403: // Forbidden
                    logger->warn("SipMessage::builder SIP Forbidden Response. Not Implemented Yet");
                    break;
                case 404: // Not Found
                    logger->warn("SipMessage::builder SIP Not Found Response. Not Implemented Yet");
                    break;
                case 500: // Server Internal Error
                    logger->warn("SipMessage::builder SIP Server Error Response. Not Implemented Yet");
                    break;
                case 501: // Not Implemented
                    logger->warn("SipMessage::builder SIP Not Implemented Response. Not Implemented Yet");
                    break;
                case 503: // Service Unavailable
                    logger->warn("SipMessage::builder SIP Service Unavailable Response. Not Implemented Yet");
                    break;
                default:
                    logger->warn("SipMessage::builder not handling response code {} [{}]", responseCode, lines[0].toString());
                    break;

            }
        }
    } else if (buffer_equals(method, "INVITE")) {

        sipMessage = new SipInviteRequest(methodProperties, headers, normalisedData.bodies);
    } else if (buffer_equals(method, "ACK")) {
        sipMessage = new SipAckRequest(methodProperties, headers);
    } else if (buffer_equals(method, "BYE")) {
        sipMessage = new SipByeRequest(methodProperties, headers);
    } else if (buffer_equals(method, "OPTIONS")) {
        sipMessage = new SipOptionsRequest(methodProperties, headers);
    } else if (buffer_equals(method, "SUBSCRIBE")) {
        sipMessage = new SipSubscribeRequest(methodProperties, headers);
    } else {
        // The Recorder is unlikely to need anything other than INVITE, ACK, BYE, OPTIONS
        logger->warn("SipMessage::builder: Not handling message method [%s] Not Implemented Yet\n", method.toString().c_str());
    }

//    if (sipMessage) {
//        sipMessage->setMessageMemory(messageBuffer);
//        sipMessage->contentLength = contentLength;
//        sipMessage->contentType = contentType;
//        sipMessage->transportType = transportType;
//    }

    return sipMessage;
}

void SipMessage::addBody(SipMessageBody::BodyType type, std::string body) {
    if (hasBody(type)) {
        logger->warn("SipMessage::addBody: Type={} already exists in message", SipMessageBody::toString(type));
        return;
    }
    else {
        bodies[type] = body;
    }
}

std::vector<SipMessageBody::BodyType> SipMessage::getBodyTypes() {
    std::vector<SipMessageBody::BodyType> list;
    for (std::map<SipMessageBody::BodyType, std::string>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
        list.push_back(it->first);
    }
    return list;
}

bool SipMessage::hasBody(SipMessageBody::BodyType bodyType) {
    return (bodies.find(bodyType) != bodies.end());
}

std::string SipMessage::getBody(SipMessageBody::BodyType bodyType) {
    std::map<SipMessageBody::BodyType, std::string>::iterator it = bodies.find((bodyType));
    if (it == bodies.end()) {
        return std::string();
    }
    return (*it).second;
}

void SipMessage::addHeader(SipHeader * header) {

};

sip_headers_t SipMessage::getHeaders() {
    sip_headers_t sipHeaders;
    for (std::map<std::string, SipHeader *>::iterator i = headers.begin(); i != headers.end(); ++i) {
        SipHeader * header = (*i).second;
        sipHeaders.push_back(header->getDescriptor());
    }
    return sipHeaders;
}

std::map<SipMessageBody::BodyType, std::string> SipMessage::getBodies() {

    return bodies;
};

string SipMessage::getCallId() {
    SipCallIdHeader * header = (SipCallIdHeader *)headers["CALL-ID"];
    if (header) {
        return header->getCallId();
    } else {
        return string();
    }
}

std::string SipMessage::getFrom() {
    SipFromHeader * header = (SipFromHeader *)headers["FROM"];
    if (header) {
        return header->getOriginalFromField();
    } else {
        return string();
    }
}

std::string SipMessage::getTo() {
    SipToHeader * header = (SipToHeader *)headers["TO"];
    if (header) {
        return header->getOriginalToField();
    } else {
        return string();
    }
}

bool SipMessage::hasBody() {
    return (bodies.size() > 0);
}

std::string SipMessage::getResponseViaContent() {
    std::map<std::string, SipHeader *>::iterator pos = headers.find("VIA");
    if (pos != headers.end()) {
        logger->debug("SipMessage::getResponseViaContent VIA response value is %s", ((SipViaHeader *)pos->second)->getResponseValue().c_str());
        return ((SipViaHeader *)pos->second)->getResponseValue();
    } else {
        logger->warn("SipMessage::getResponseViaContent Failed to find VIA header");
        return string();
    }
}

SipMessageBody::BodyType SipMessage::getContentType() {
    std::map<std::string, SipHeader *>::iterator pos = headers.find("CONTENT-TYPE");
    if (pos != headers.end()) {
        return ((SipContentTypeHeader *) pos->second)->getContentType();
    }
    logger->warn("SipMessage::getContentType Did not find Content-Type header");
    return SipMessageBody::BodyType::NONE;
}

int SipMessage::getContentLength() {
    std::map<std::string, SipHeader *>::iterator pos = headers.find("CONTENT-LENGTH");
    if (pos != headers.end()) {
        return ((SipContentLengthHeader *)pos->second)->getContentLength();
    }
    logger->warn("SipMessage::getContentLength Did not find Content-Length header\n");
    return 0;
}

string SipMessage::getRemoteAddress() {
    SipViaHeader * header = (SipViaHeader *)headers["VIA"];
    if (header) {
        return header->getRemoteAddress();
    } else {
        return string();
    }
}

int SipMessage::getRemotePort() {
    SipViaHeader * header = (SipViaHeader *)headers["VIA"];
    if (header) {
        return header->getRemotePort();
    } else {
        return 5060;
    }
}

SipMessage::SipMessageTransportType SipMessage::getTransportType() {
    SipViaHeader * header = (SipViaHeader *)headers["VIA"];
    if (header) {
        return header->getTransportType();
    } else {
        return DefaultTransportType;
    }
}

string SipMessage::getCSequence() {
    SipCSeqHeader * header = (SipCSeqHeader *)headers["CSEQ"];
    if (header) {
        return header->getCSeq();
    } else {
        return string();
    }
}





