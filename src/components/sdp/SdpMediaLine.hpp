//
// Created by simon on 12/05/17.
//

#ifndef SDPMEDIALINE_HPP
#define SDPMEDIALINE_HPP

#include "../utilities/buffers.h"
#include "../media/definitions.h"

#include "SdpAttributeLine.hpp"
#include "SdpFormatType.hpp"
#include "SdpMediaType.hpp"
#include "SdpProtocolType.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

/// @brief handles the SDP media line
/// @details m=<media> <port> <proto> <fmt> ...
/// @details <media> is the media type.  Currently defined media are "audio", "video", "text", "application", and "message"
/// @details <port> is the transport port to which the media stream is sent.  The meaning of the transport port depends on the network being used as specified in the relevant "c=" field, and on the transport protocol defined in the <proto> sub-field of the media field.
/// @details <proto> is the transport protocol. 'udp', 'RTP/AVP', 'RTP/SAVP'
/// @details <fmt> is a media format description.  The fourth and any subsequent sub-fields describe the format of the media. The interpretation of the media format depends on the value of the <proto> sub-field.
class SdpMediaLine {

public:

    SdpMediaLine(mapped_buffer_t line) {
        std::vector<mapped_buffer_t> content = buffer_split(line, ' ');
        size_t  itemCount = content.size();
        if (itemCount >= 4) {
            type = SdpMediaType(content[MediaIndexMediaType]);
            std::vector<mapped_buffer_t> portInfo = buffer_split(content[MediaIndexTransportPort], '/');
            port = toUnsignedLongInteger(portInfo[0]);
            if (portInfo.size() > 1) {
                portCount = toUnsignedLongInteger(portInfo[1]);
            }
            protocol = SdpProtocolType(content[MediaIndexTransportProtocol]);
            for (size_t i = MediaIndexFirstFormatItem; i < itemCount; ++i) {
                this->formats.push_back(toUnsignedShortInteger(content[i]));
            }
        } else {
            //  get the logger here to avoid getting it every time we get a media line when logging not required
            auto logger = spdlog::get("stdlogger");
            logger->warn("SDP::Media header line invalid, expecting at least 4 items, have {}, line={}",
                    content.size(), bufferToString(line).c_str());
        }
    }

    SdpMediaLine(const SdpMediaLine &other)
            : type(other.type), port(other.port), portCount(other.portCount), protocol(other.protocol) { }

public:

    std::string toString() {
        std::stringstream ss;
        ss << type.toString();
        ss << ' ';
        ss << port;
        ss << ' ';
        ss << protocol.toString();
        for (auto i = formats.begin(); i != formats.end(); ++i) {
            ss << ' ';
            ss << *i;
        }
        return ss.str();
    }

public:

    SdpMediaType type;
    uint16_t port;
    uint16_t portCount = 1;
    SdpProtocolType protocol = SdpProtocolType();
    std::vector<uint16_t > formats = std::vector<uint16_t >();
};

#endif //SDPMEDIALINE_HPP
