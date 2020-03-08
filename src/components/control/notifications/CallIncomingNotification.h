//
// Created by Simon Willis on 21/05/2016.
//

#ifndef CORE_NEWCALLNOTIFICATION_H
#define CORE_NEWCALLNOTIFICATION_H

#include "Notification.hpp"

class CallIncomingNotification : public Notification {

public:
    CallIncomingNotification(const uint32_t channelId, const char *callId, const char * tag, const char * to, const char * from, const sip_headers_t headers, const int errorCode = 0, const char * errorMessage = "OK", const int dialogueId = 0)
            : Notification("callIncoming", channelId, errorCode, errorMessage, dialogueId)
            , callId(callId), tag(tag), to(to), from(from), headers(headers) {

    }

private:
    Json::Value parseViaHeader(std::string viaHeader) {
        Json::Value result;
        mapped_buffer source = mapped_buffer(viaHeader);
        std::vector<mapped_buffer> parts = buffer_split(source, ';');

        for (size_t i = 0; i < parts.size(); ++i) {
            if (i == 0) {
                auto subParts = buffer_split_on_first(parts[i], ' ');
                if (subParts.size() > 1) {
                    result["proto"] = buffer_contains(subParts[0], "TCP") ? "TCP":"UDP";
                    auto addressParts = buffer_split_on_first(subParts[1], ':');
                    result["remoteServer"] = addressParts[0].toString();
                    auto addressOctets = buffer_split(addressParts[0], '.');
                    if (addressOctets.size() == 4) {
                        auto isIpAddress = true;
                        for (int octet = 0; octet < 4; ++octet) {
                            auto val = toUnsignedLongInteger(addressOctets[octet]);
                            if ((val > 255) || (octet == 0 && val == 0)) {
                                isIpAddress = false;
                                break;
                            }
                        }
                        if (isIpAddress) {
                            result["remoteIpAddress"] = addressParts[0].toString();
                            if (addressParts[1].len) {
                                result["remotePort"] = addressParts[1].toString();
                            }
                            result["valid"] = true;
                        }
                    }
                }
            } else {
                auto subParts = buffer_split_on_first(parts[i], '=');
                if (buffer_starts_with(subParts[0], "received")) {
                    if (subParts[1].len) {
                        result["remoteIpAddress"] = subParts[1].toString();
                        result["valid"] = true;
                    }
                } else if (buffer_starts_with(subParts[0], "rport")) {
                    if (subParts[1].len) {
                        result["remotePort"] = subParts[1].toString();
                    }
                } else {
                    if (subParts[1].len) {
                        result[subParts[0].toString()] = subParts[1].toString();
                    }
                }
            }
        }
        return result;
    }

protected:
    bool hasData() override {
        return true;
    }

    Json::Value getData() override {
        Json::Value data;
        Json::Value sipHeaders;
        Json::Value remoteConnectionDetails;

        data["callId"] = callId;
        data["to"] = to;
        data["from"] = from;

        for (sip_headers_t::iterator i = headers.begin(); i != headers.end(); i++) {
            sipHeaders[(*i).first.c_str()] = (*i).second.c_str();

            if (! ((*i).first).compare("Via")) {
                remoteConnectionDetails = parseViaHeader((*i).second);
                data["remoteConnectionDetails"] = remoteConnectionDetails;
            }
        }

        data["remoteConnectionDetails"] = remoteConnectionDetails;
        data["sipHeaders"] = sipHeaders;

        return data;
    }

private:

    //TODO Add Call Data to CallIncomingNotification
    std::string callId;
    const std::string tag;
    const std::string to;
    const std::string from;
    sip_headers_t headers;

};

#endif //CORE_NEWCALLNOTIFICATION_H
