//
// Created by simon on 12/05/17.
//

#ifndef SDPORIGINLINE_HPP
#define SDPORIGINLINE_HPP


#include <sstream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "../utilities/buffers.h"
#include "SdpAddrType.hpp"
#include "../media/definitions.h"

/// @brief o=<username> <sess-id> <sess-version> <nettype> <addrtype>
/// The "o=" field gives the originator of the session (her username and
/// the address of the user's host) plus a session identifier and version number:
class SdpOriginLine {

public:

    SdpOriginLine() {}

    SdpOriginLine(mapped_buffer_t line) {
        std::vector<mapped_buffer_t> content = buffer_split(line, ' ');
        if (content.size() == 6) {
            username = bufferToString(content[OriginIndexUsername]);
            sessionId = bufferToString(content[OriginIndexSessionId]);
            sessionVersion = toLongInteger(content[OriginIndexSessionVersion]);
            netType = bufferToString(content[OriginIndexNetType]);
            addrType = buffer_equals(content[OriginIndexAddressType], "IP6") ? SdpAddrType(SdpAddrType::Type::IPV6) : SdpAddrType(SdpAddrType::Type::IPV4);
            address = bufferToString(content[OriginIndexAddress]);
        } else {
            spdlog::get("stdlogger")->warn("Failed to load SDP Origin data content has {} entries not 6", content.size());
        }
    }

    SdpOriginLine(const SdpOriginLine &other)
            : username(other.username), sessionId(other.sessionId), sessionVersion(other.sessionVersion), netType(other.netType), address(other.address), addrType(other.addrType) { }


public:

    std::string username;
    std::string sessionId;
    std::uint32_t sessionVersion;
    std::string netType = "IN";
    std::string address;
    SdpAddrType addrType;

};

#endif //SDPORIGINLINE_HPP
