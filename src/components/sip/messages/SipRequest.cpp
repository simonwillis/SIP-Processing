//
// Created by Simon Willis on 31/05/2016.
//
#include <sstream>

#include "SipRequest.h"
#include "SipInviteRequest.h"
#include "../headers/SipViaHeader.h"
#include "../headers/SipFromHeader.h"
#include "../headers/SipToHeader.h"
#include "../headers/SipDateHeader.h"
#include "../headers/SipCallIdHeader.h"
#include "../headers/SipSupportedHeader.h"
#include "../headers/SipRequireHeader.h"
#include "../headers/SipMinSeHeader.h"
#include "../headers/SipUserAgentHeader.h"
#include "../headers/SipAllowHeader.h"
#include "../headers/SipCSeqHeader.h"
#include "../headers/SipMaxForwardsHeader.h"
#include "../headers/SipTimestampHeader.h"
#include "../headers/SipContactHeader.h"
#include "../headers/SipExpiresHeader.h"
#include "../headers/SipAllowEventsHeader.h"
#include "../headers/SipContentTypeHeader.h"
#include "../headers/SipMimeVersionHeader.h"
#include "../headers/SipContentLengthHeader.h"

using namespace std;

SipRequest::SipRequest() {

}

SipRequest::SipRequest(mapped_buffer_t propertiesLine, std::map<std::string, SipHeader *> headers, std::map<SipMessageBody::BodyType, std::string> bodies)
        : SipMessage(propertiesLine, headers, bodies) {
    // pull the basics that should be valid for all requests
    logger = spdlog::get("stdlogger");
}


std::string SipRequest::buildOkResponse(std::string localTag) {

    return buildStandardFullResponse("200 OK", localTag);
}

stringstream SipRequest::buildStandardBaseResponse(const char * method, string localTag) {

    std::stringstream ss;

    ss << "SIP/2.0 " << method << "\r\n";
    ss << "Via: " << getResponseViaContent() << "\r\n";
    ss << "From: " << getFrom() << "\r\n";
    ss << "To: " << getTo() << ";tag=" << localTag << "\r\n";
    ss << "Call-ID: " << getCallId() << "\r\n";
    ss << "CSeq: " << getCSequence() << "\r\n";
    ss << "Contact: " << "<sip:me@192.168.0.4>" << "\r\n";
    ss << "User-Agent: " << "Core-0-1" << "\r\n";

    return ss;
}

string SipRequest::buildStandardFullResponse(const char * method, string localTag) {

    stringstream ss = buildStandardBaseResponse(method, localTag);

    ss << "Content-Length: 0" << "\r\n";
    ss << "\r\n";

    return ss.str();
}