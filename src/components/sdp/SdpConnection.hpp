//
// Created by simon on 21/05/17.
//

#ifndef SDPCONNECTION_HPP
#define SDPCONNECTION_HPP

#include <cstdio>

#include "SdpConnectionLine.hpp"
#include "SdpAddrType.hpp"

class SdpConnection {

public:

    SdpConnection() { }

    SdpConnection(std::string address, SdpAddrType addrType = SdpAddrType::IPV4, std::string netType = "IN")
            : address(address), addrType(addrType), netType(netType) { }

    SdpConnection(const SdpConnectionLine &line)
            : address(line.address), addrType(line.addrType), netType(line.netType) { }

public:

    std::string toString() {
        std::stringstream ss;
        ss << "c=";
        ss << netType << ' ';
        ss << addrType.toString() << ' ';
        ss << address;
        return ss.str();
    }

    std::string toJson() {
        std::stringstream ss;
        ss << "{";
        ss << "\"netType\":\"" << netType << "\",";
        ss << "\"addressType\":\"" << addrType.toString() << "\",";
        ss << "\"address\":\"" << address << "\"";
        ss << "}";
        return ss.str();
    }

private:

    std::string address;
    SdpAddrType addrType = SdpAddrType::IPV4;
    std::string netType = "IN";

};

#endif //SDPCONNECTION_HPP
