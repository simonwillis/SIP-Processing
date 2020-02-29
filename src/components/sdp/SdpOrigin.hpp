//
// Created by simon on 16/05/17.
//

#ifndef SDPORIGIN_HPP
#define SDPORIGIN_HPP

#include <cstdio>
#include <ctime>
#include "SdpOriginLine.hpp"

class SdpOrigin {

public:

    SdpOrigin() {
        std::time_t t = std::time(0);
        sessionId = (uint64_t) t;
    }

    SdpOrigin(std::string username, std::string sessionId, uint32_t  sessionVersion, std::string address, SdpAddrType addrType = SdpAddrType())
            : username(username), sessionVersion(sessionVersion), address(address), addrType(addrType) {

        setSessionId(sessionId);
    }

    SdpOrigin(std::string username, uint64_t sessionId, uint32_t  sessionVersion, std::string address, SdpAddrType addrType = SdpAddrType())
            : username(username), sessionId(sessionId), sessionVersion(sessionVersion), address(address), addrType(addrType) { }

    SdpOrigin(const SdpOriginLine & line)
            : username(line.username), sessionVersion(line.sessionVersion), address(line.address), addrType(line.addrType) {

        setSessionId(line.sessionId);
    }

    SdpOrigin(const SdpOrigin &other)
            : username(other.username), sessionId(other.sessionId), sessionVersion(other.sessionVersion), address(other.address), addrType(other.addrType), netType(other.netType) { }


public:

    void setSessionId(uint32_t id) {
        sessionId = id;
    }
    void setSessionId(std::string id) {
        sessionId = std::strtoull(id.c_str(), NULL, 10);
    }
    uint32_t getSessionId() {
        return sessionId;
    }

public:

    std::string toString() {
        std::stringstream ss;
        ss << "o=";
        ss << username << ' ';
        ss << sessionId << ' ';
        ss << sessionVersion << ' ';
        ss << netType << ' ';
        ss << addrType.toString() << ' ';
        ss << address;
        return ss.str();
    }

    std::string toJson() {
        std::stringstream ss;

        ss << "{";
        ss << "\"username\":\"" << username << "\",";
        ss << "\"sessionId\":" << sessionId << ",";
        ss << "\"sessionVersion\":" << sessionVersion << ",";
        ss << "\"networkType\":\"" <<  netType << "\",";
        ss << "\"addressType\":\"" <<  addrType.toString() << "\",";
        ss << "\"address\":\"" << address << "\"";
        ss << "}";

        return ss.str();
    }

private:

    std::string username;
    std::uint64_t sessionId;
    std::uint32_t sessionVersion;
    std::string address;
    SdpAddrType addrType;
    std::string netType = "IN";
};
#endif //SDPORIGIN_HPP
