//
// Created by simon on 07/11/16.
//

#ifndef _SDP_HPP_
#define _SDP_HPP_

#include <iostream>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "../utilities/buffers.h"
#include "../../../lib/json/json.h"
#include "../media/definitions.h"
#include "SdpOriginLine.hpp"
#include "SdpAttributeLine.hpp"
#include "SdpMediaLine.hpp"
#include "SdpConnectionLine.hpp"
#include "SdpTiming.hpp"
#include "SdpFormatType.hpp"
#include "SdpMedia.hpp"
#include "SdpAttribute.hpp"
#include "SdpOrigin.hpp"
#include "SdpConnection.hpp"
#include "SdpVersion.hpp"
#include "SdpSession.hpp"


class Sdp {

public:
    static const size_t MaxDataSize = 4096;


public:

    Sdp();

    Sdp(const char * rawSdpBodyData, size_t size);

    Sdp(Sdp &other)
        : streamCount(other.streamCount), session(other.session), origin(other.origin),connection(other.connection), timing(other.timing) {

        this->logger = spdlog::get("stdlogger");
        attributes.reserve(other.attributes.size());
        for (std::vector<SdpAttribute>::iterator i = other.attributes.begin(); i != other.attributes.end(); ++i) {
            this->attributes.push_back(*i);
        }

        //this->media.reserve(other.media.size());
        for (std::vector<SdpMedia>::iterator i = other.media.begin(); i != other.media.end(); ++i) {
            SdpMedia m(*i);
            this->media.push_back(m);
        }

        memcpy(&data, &other.data, MaxDataSize);
    };

public:

    std::string toString();
    std::string toJson();

    void setOrigin(SdpOrigin origin);
    void setConnection(SdpConnection &connection);
    uint16_t addMediaStream(SdpMedia &media);
    uint16_t addMediaStream(SdpMediaLine &mediaLine);
    void addFormat(uint16_t streamId, SdpFormat format);
    void addAttribute(std::string &name, std::string &value, std::string &parameters);
    void addAttribute(std::string & name);
    void addAttribute(uint16_t streamId, std::string &name, std::string &value, std::string &parameters);
    void addAttribute(uint16_t streamId, std::string &name);
    void addAttribute(SdpAttribute &attribute);
    void addAttribute(uint16_t streamId, SdpAttributeLine &attribute);

    void setSessionId(uint32_t id) { origin.setSessionId(id); }
    uint32_t getSessionId() { return origin.getSessionId(); }
    uint8_t getStreamCount() { return streamCount; }

    friend std::ostream& operator << (std::ostream& os, Sdp& sdp);

private:

    uint8_t streamCount = 0;
    SdpVersion version;
    SdpOrigin origin;
    SdpSession session;
    SdpConnection connection;
    SdpTiming timing;

    std::vector<SdpAttribute> attributes;
    std::vector<SdpMedia> media;

    // holds the raw SDP against which the SDP values are mapped
    char data[MaxSdpBodyLength];

    std::shared_ptr<spdlog::logger> logger;


};


#endif //_SDP_HPP_
