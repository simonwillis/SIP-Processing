//
// Created by simon on 16/05/17.
//

#ifndef SDPMEDIA_HPP
#define SDPMEDIA_HPP

#include <vector>
#include "SdpAttribute.hpp"
#include "SdpFormat.hpp"

class SdpMedia {

public:

    SdpMedia() {
        logger = spdlog::get("stdlogger");
    }

    SdpMedia(SdpMediaLine &line)
            : type(line.type), port(line.port), protocol(line.protocol) { }

    SdpMedia(SdpMediaType type, uint16_t port, SdpProtocolType protocol)
            : type(type), port(port), protocol(protocol) { }

    SdpMedia(const SdpMedia &other) {
        type = other.type;
        port = other.port;
        protocol = other.protocol;

        // Don't use iterators here, the constness of the pointers is a total PITA
        size_t formatCount = other.formats.size();
        formats.reserve(other.formats.size());
        for (size_t i = 0; i < formatCount; ++i) {
            formats.push_back(other.formats[i]);
        }
        size_t attributeCount = other.attributes.size();
        attributes.reserve(attributeCount);
        for (size_t  i = 0; i < attributeCount; ++i) {
            attributes.push_back(other.attributes[i]);
        }
    }

public:

    void addFormat(SdpFormat format) {
        if (hasFormat(format.getFmt())) {
            logger->warn("SdpMedia::addFormat Format {} already exists in media", format.getFmt());
        }
        formats.push_back(format);
    }

    bool hasFormat(uint16_t fmtId) {
        SdpFormatType type = SdpFormatType(fmtId);
        for (std::vector<SdpFormat>::iterator i = formats.begin(); i != formats.end(); ++i) {
            if ((*i).getFmt() == type.getFmtNumber()) {
                return true;
            }
        }
        return false;
    }

    void addAttribute(SdpAttribute attribute) {
        attributes.push_back(attribute);
    }

    void addAttribute(const char * key, const char * value, const char * parameters = nullptr) {

        logger->debug("SdpMedia::addAttribute [key={}] [value={}] [parameters={}]", key ? key:"undefined", value ? value:"undefined", parameters ? parameters:"undefined");

        SdpAttribute attribute(key, value, parameters);
        attributes.push_back(attribute);
    }

    std::string toString() {
        size_t formatCount = formats.size();
        std::stringstream ss;
        ss << "m=";
        ss << type.toString();
        ss << ' ';
        ss << port;
        ss << ' ';
        ss << protocol.toString();
        for (size_t i = 0; i < formatCount; ++i) {
            ss << ' ' << formats[i].getFmt();
        }
        if (attributes.size()) {
            for (std::vector<SdpAttribute>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
                ss << "\r\n" << (*it).toString();
            }
        }
        return ss.str();
    }

    std::string toJson() {
        size_t formatCount = formats.size();
        bool firstAttribute = true;
        std::stringstream ss;
        ss << "{";
        ss << "\"type\":\"" << type.toString() << "\",";
        ss << "\"port\":" << port << ",";
        ss << "\"protocol\":\"" << protocol.toString() << "\",";
        ss << "\"attribute\":[";
        for (std::vector<SdpAttribute>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
            if (! firstAttribute) {
                ss << ',';
            }
            firstAttribute = false;
            ss << "\r\n" << (*it).toJson();
        }
        ss << "]";
        ss << "}";

        return ss.str();
    }

public:

    SdpMediaType type;
    uint16_t port;
    SdpProtocolType protocol = SdpProtocolType::rtp;
    std::vector<SdpFormat> formats = std::vector<SdpFormat>();
    std::vector<SdpAttribute> attributes = std::vector<SdpAttribute>();

private:
    std::shared_ptr<spdlog::logger> logger;

};

#endif //SDPMEDIA_HPP
