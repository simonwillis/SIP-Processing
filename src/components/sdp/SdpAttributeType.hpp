//
// Created by simon on 16/05/17.
//

#ifndef SDPATTRIBUTETYPE_HPP
#define SDPATTRIBUTETYPE_HPP

#include "../utilities/buffers.h"

class SdpAttributeType {

public:

    enum Type {
        none,
        rtpmap,
        ptime,
        recvonly,
        sendonly,
        sendrecv,
        inactive,
        orient,
        conferencetype,
        lang,
        sdplang,
        framerate,
        quality,
        fmtp
    };

public:

    SdpAttributeType() { type = none; }

    SdpAttributeType(Type value) { type = value; }

    SdpAttributeType(std::string value) { setType(value); }

    SdpAttributeType(mapped_buffer_t value) { setType(value); }

    SdpAttributeType(const SdpAttributeType &other) : type(other.type) { }

public:

    std::string toString() {
        switch (type) {
            case rtpmap: return "rtpmap";
            case ptime: return "ptime";
            case recvonly: return "recvonly";
            case sendonly: return "sendonly";
            case sendrecv: return "sendrecv";
            case inactive: return "inactive";
            case orient: return "orient";
            case conferencetype: return "conferencetype";
            case lang: return "lang";
            case sdplang: return "sdplang";
            case framerate: return "framerate";
            case quality: return "quality";
            case fmtp: return "fmtp";
            default: return "none";
        }
    }

    Type getType() { return type; }

private:

    void setType(mapped_buffer_t value) {
        mapped_buffer_t buffer = buffer_trim(value);
        if (buffer_equals(buffer, "rtpmap")) {
            type = Type::rtpmap;
        } else if (buffer_equals(buffer, "ptime")) {
            type = Type::ptime;
        } else if (buffer_equals(buffer, "recvonly")) {
            type = Type::recvonly;
        } else if (buffer_equals(buffer, "sendonly")) {
            type = Type::sendonly;
        } else if (buffer_equals(buffer, "sendrecv")) {
            type = Type::sendrecv;
        } else if (buffer_equals(buffer, "inactive")) {
            type = Type::inactive;
        } else if (buffer_equals(buffer, "orient")) {
            type = Type::orient;
        } else if (buffer_equals(buffer, "conferencetype")) {
            type = Type::conferencetype;
        } else if (buffer_equals(buffer, "lang")) {
            type = Type::lang;
        } else if (buffer_equals(buffer, "sdplang")) {
            type = Type::sdplang;
        } else if (buffer_equals(buffer, "framerate")) {
            type = Type::framerate;
        } else if (buffer_equals(buffer, "quality")) {
            type = Type::quality;
        } else if (buffer_equals(buffer, "fmtp")) {
            type = Type::fmtp;
        } else {
            type = Type::none;
        }

    }
    void setType(std::string value) {
        mapped_buffer_t buffer = mapped_buffer(value);
        setType(buffer);
    }
private:
    Type type = none;
};



#endif //SDPATTRIBUTETYPE_HPP
