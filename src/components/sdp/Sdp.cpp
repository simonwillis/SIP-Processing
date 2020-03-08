//
// Created by simon on 07/11/16.
//

#include "Sdp.h"

using namespace std;

#define PRESENTED_IP_ADDRESS "192.168.0.4"


std::ostream& operator<< (std::ostream& os, Sdp& sdp) {
    os << sdp.toString();
    return os;
}

Sdp::Sdp() {
    logger = spdlog::get("stdlogger");

    attributes.reserve(32);
    media.reserve(8);
}


Sdp::Sdp(const char * rawSdpBodyData, size_t size) {

    logger->debug("New DSP item raw bodydata, size={} content={}", size, std::string(rawSdpBodyData, size));

    size_t dataSize = (size < MaxSdpBodyLength) ? size : MaxSdpBodyLength;
    memcmp(rawSdpBodyData, data, dataSize);
    mapped_buffer_t buffer = mapped_buffer(rawSdpBodyData, dataSize);
    vector<mapped_buffer_t> lines = buffer_split_lines(buffer);
    size_t lineCount = lines.size();

    for (int lineIndex = 0; lineIndex < lineCount; ++lineIndex) {
        vector<mapped_buffer_t> keyValuePair = buffer_split_on_first(lines[lineIndex], '=');

        if (keyValuePair.size() == 2) {
            char c = tolower(keyValuePair[0].buf[0]);

            switch(c) {
                case 'o': {
                        SdpOriginLine line(keyValuePair[1]);
                        origin = SdpOrigin(line);
                    }
                    break;
                case 'v':
                    // ignore as irrelevant for now
                    break;
                case 's':
                    session = SdpSession(keyValuePair[1]);
                    break;
                case 'c': {
                        SdpConnection line(keyValuePair[1]);
                        connection = SdpConnection(line);
                    }
                    break;
                case 't':
                    timing = SdpTiming(keyValuePair[1]);
                break;
                case 'm': {
                        SdpMediaLine line(keyValuePair[1]);
                        addMediaStream(line);
                    }
                    break;
                case 'a': {
                        SdpAttributeLine line(keyValuePair[1]);
                        addAttribute(streamCount, line);
                    }
                    break;
                default:
                    logger->warn("SDP: Don't understand entry '{}' starting with '{}'", lines[lineIndex].toString(), c);
                    break;
            }

        } else {
            logger->debug("Invalid SDP line, not split by '=', value is '{}'", lines[lineIndex].toString());
        }
    }

    vector<mapped_buffer_t> ownerData;
}

void Sdp::setOrigin(SdpOrigin o) {
    origin = o;
}

void Sdp::setConnection(SdpConnection &c) {
    connection = c;
}

uint16_t Sdp::addMediaStream(SdpMedia &mediaItem) {
    ++streamCount;

    SdpMedia m = mediaItem;
    this->media.push_back(m);

    return streamCount;
}

uint16_t Sdp::addMediaStream(SdpMediaLine &line) {
    ++streamCount;

    logger->debug("Sdp::addMediaStream {}, {}", streamCount, line.toString().c_str());

    SdpMedia m(line);
    this->media.push_back(m);

    return streamCount;
}

void Sdp::addAttribute(uint16_t streamId, SdpAttributeLine &attributeLine) {

    if (streamId == 0) {
        // common attribute
        SdpAttribute commonAttribute = SdpAttribute(attributeLine);
        attributes.push_back(commonAttribute);

        return;
    }
    uint16_t mediaIndex = streamId - 1;
    if (mediaIndex >= streamCount) {
        logger->warn("Invalid media stream id {} referenced when adding an attribute", streamId);
        return;
    }
    media[mediaIndex].addAttribute(attributeLine);
}

void Sdp::addFormat(uint16_t streamId, SdpFormat format) {

    uint16_t mediaIndex = streamId - 1;

    if (mediaIndex >= streamCount) {
        logger->warn("Invalid media stream id {} referenced when adding a format", streamId);
        return;
    }
    media[mediaIndex].addFormat(format);
}

void Sdp::addAttribute(SdpAttribute &attribute) {
    this->attributes.push_back(attribute);
}

void Sdp::addAttribute(std::string &name, std::string &value, std::string &parameters) {
    SdpAttribute a(name, value, parameters);
    attributes.push_back(a);
}

void Sdp::addAttribute(std::string & name) {
    SdpAttribute a(name);
    attributes.push_back(a);
}

void Sdp::addAttribute(uint16_t streamId, std::string &name, std::string &value, std::string &parameters) {
    uint16_t mediaIndex = streamId - 1;

    if (mediaIndex >= streamCount) {
        logger->debug("Invalid media stream id {} referenced when adding an attribute", streamId);
        return;
    }

    SdpAttribute a(name, value, parameters);
    media[mediaIndex].addAttribute(a);
}

void Sdp::addAttribute(uint16_t streamId, std::string &name) {
    uint16_t mediaIndex = streamId - 1;

    if (mediaIndex >= streamCount) {
        logger->debug("Invalid media stream id {} referenced when adding an attribute, name={}", streamId, name);
        return;
    }

    SdpAttribute a(name);
    media[mediaIndex].addAttribute(a);
}

/***
 *
 * @return string containing JSON
 *
 *   Minimum Response Guarantee
 *   {
 *       owner: {},
 *       session: {},
 *       media: [],
 *       connection: {},
 *       time: {}
 *   }
 */
std::string Sdp::toJson() {

    size_t attributeCount = attributes.size();
    size_t attributeIndex;

    stringstream ss;

    ss << "{";
    ss << "\"version\":" << version.toJson() << ',';
    ss << "\"owner\":" << origin.toJson() << ',';
    ss << "\"session\":" << session.toJson() << ',';
    ss << "\"connection\":" << connection.toJson() << ',';
    ss << "\"time\":" << timing.toJson() << ',';

    for (attributeIndex = 0; attributeIndex < attributeCount; ++attributeIndex) {
        ss << attributes[attributeIndex].toJson() << ',';
    }

    ss << "\"media\":[";
    for (uint8_t mediaIndex = 0; mediaIndex < streamCount; ++mediaIndex) {
        if (mediaIndex) {
            ss << ',';
        }
        ss << media[mediaIndex].toJson();
    }
    ss << "]";

    ss << "}";

    return ss.str();
}

std::string Sdp::toString() {
    size_t attributeCount = attributes.size();
    size_t attributeIndex;
    stringstream ss;
    ss << version.toString() << "\r\n";
    ss << origin.toString() << "\r\n";
    ss << session.toString() << "\r\n";
    ss << connection.toString() << "\r\n";
    ss << timing.toString() << "\r\n";
    for (attributeIndex = 0; attributeIndex < attributeCount; ++attributeIndex) {
        ss << attributes[attributeIndex].toString() << "\r\n";
    }
    for (uint8_t mediaIndex = 0; mediaIndex < streamCount; ++mediaIndex) {
        ss << media[mediaIndex].toString() << "\r\n";
    }
    return ss.str();
}


