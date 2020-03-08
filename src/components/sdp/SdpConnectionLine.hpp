//
// Created by simon on 12/05/17.
//

#ifndef UDPSEND_SDPCONNECTION_HPP
#define UDPSEND_SDPCONNECTION_HPP

#include <sstream>
#include "../utilities/buffers.h"
#include "../media/definitions.h"


class SdpConnectionLine {

public:

    SdpConnectionLine() : address(std::string()), addrType(SdpAddrType::IPV4), netType("IN") { }

    SdpConnectionLine(std::string address, SdpAddrType addrType = SdpAddrType::IPV4, std::string netType = "IN")
            : address(address), addrType(addrType), netType(netType) { }

    SdpConnectionLine(mapped_buffer_t line) {
        std::vector<mapped_buffer_t> content = buffer_split(line, ' ');
        netType = bufferToString(content[ConnectionIndexNetType]);
        addrType = SdpAddrType(content[ConnectionIndexAddressType]);
        address = bufferToString(content[ConnectionIndexAddress]);
    }

    SdpConnectionLine(const SdpConnectionLine &other) : address(other.address), addrType(other.addrType), netType(other.netType) { }

public:

    std::string toString() {
        std::stringstream ss;
        ss << netType << ' ';
        ss << addrType.toString() << ' ';
        ss << address;
        return ss.str();
    }

public:

    std::string address;
    SdpAddrType addrType = SdpAddrType::IPV4;
    std::string netType;

};


#endif //UDPSEND_SDPCONNECTION_HPP
