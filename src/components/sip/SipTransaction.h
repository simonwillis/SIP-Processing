//
// Created by Simon Willis on 31/05/2016.
//

#ifndef SIP_SIPTRANSACTION_H
#define SIP_SIPTRANSACTION_H


#include <sstream>

class SipTransaction {
public:
    SipTransaction()
            : channelId(0), sipRequest(nullptr), sdpOffer(std::string()) {}
    SipTransaction(uint32_t channelId, SipRequest * sipRequest, std::string sdpOffer)
            : channelId(channelId), sipRequest(sipRequest), sdpOffer(sdpOffer) {}

    SipRequest::SipMessageType getType () {
        if (sipRequest) {
            return sipRequest->getRequestType();
        }
        return SipRequest::SipMessageType::UNKNOWN;
    }

    std::string toString() {
        std::stringstream ss;
        ss << "Transaction";
        ss << " [channelId=" << channelId << "]";
        ss << " [Request=" << (sipRequest ? sipRequest->getTypeDescriptionString() : "NONE") << "]";
        ss << " [SdpOffer=" << sdpOffer << "]";
        return ss.str();
    }

    uint32_t channelId;
    SipRequest * sipRequest;
    std::string sdpOffer;
};


#endif //SIP_SIPTRANSACTION_H
