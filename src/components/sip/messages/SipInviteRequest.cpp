//
// Created by Simon Willis on 06/06/2016.
//

#include <sstream>
#include "SipInviteRequest.h"
#include "SipMessage.h"

using namespace std;

void SipInviteRequest::isReinvite(bool flag) {
    this->reinviteFlag = flag;
}

bool SipInviteRequest::isReinvite() {
    return this->reinviteFlag;
};

string SipInviteRequest::buildTryingResponse(string localTag) {

    return buildStandardFullResponse("100 Trying", localTag);
}

string SipInviteRequest::buildRingingResponse(string localTag) {

    return buildStandardFullResponse("180 Ringing", localTag);
}

string SipInviteRequest::buildAnswerResponse(string localTag, std::string sdpAnswer) {

    std::stringstream ss = buildStandardBaseResponse("200 OK", localTag);

    ss << "Accept: " << "application/sdp" << "\r\n";
    ss << "Allow: " << "INVITE, ACK, BYE, CANCEL, INFO, PRACK, REFER, UPDATE, NOTIFY" << "\r\n";
    ss << "Supported: " << "siprec, record-aware" << "\r\n";
    ss << "Content-Type: " << "application/sdp" << "\r\n";
    ss << "Content-Disposition: " << "session" << "\r\n";
    ss << "Content-Length: " << sdpAnswer.length() << "\r\n";
    ss << "\r\n";
    ss << sdpAnswer;

    return ss.str();
}

string SipInviteRequest::buildServiceUnavailableResponse(string localTag, uint32_t retryAfter) {

    std::stringstream ss = buildStandardBaseResponse("503 Service Unavailable", localTag);

    ss << "Retry-After: " << retryAfter << "\r\n";
    ss << "Content-Length: 0" << "\r\n";
    ss << "\r\n";

    return ss.str();
}

string SipInviteRequest::buildMovedTemporarilyRespose(std::string localTag, std::string alternateAddress, uint32_t expiresAfter) {

    std::stringstream ss = buildStandardBaseResponse("302 Moved Temporarily", localTag);

    ss << "Contact: " << alternateAddress << ";expires=" << expiresAfter << "\r\n";
    ss << "Content-Length: 0" << "\r\n";
    ss << "\r\n";

    return ss.str();
}

