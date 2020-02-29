//
// Created by Simon Willis on 15/05/2017.
//

#ifndef SDPPROTOCOLDATA_H
#define SDPPROTOCOLDATA_H

#include "../utilities/buffers.h"

class SdpProtocolType {

public:

    enum Type {
        udp, // 'udp' an unspecified protocol running over UDP
        rtp, // 'RTP/AVP' Standard RTP for audio and video
        srtp // 'RTP/SAVP' Secure Real Time Protocol over UDP
    };

public:

    SdpProtocolType()
            : type(rtp) { }

    SdpProtocolType(Type value)
            : type(value) {}

    SdpProtocolType(std::string value) { setType(value); }

    SdpProtocolType(mapped_buffer_t value) { setType(value); }

    SdpProtocolType(const SdpProtocolType &other)
            : type(other.type) { }

public:

    std::string toString() {
        switch (type) {
            case udp: return "UDP";
            case rtp: return "RTP/AVP";
            case srtp: return "RTP/SAVP";
            default: return "RTP/AVP";
        }
    }

    Type getType() { return type; }

private:

    void setType(mapped_buffer_t value) {
        mapped_buffer_t protocolType = buffer_trim(value);
        if (buffer_equals(protocolType, "RTP/AVP")) {
            type = rtp;
        } else if (buffer_equals(protocolType, "RTP/SAVP")) {
            type = srtp;
        } else if (buffer_equals(protocolType, "UDP")) {
            type = udp;
        } else {
            type = rtp;
        }
    }

    void setType(std::string value) {
        mapped_buffer_t buffer = mapped_buffer(value);
        setType(buffer);
    }

private:

    Type type;

};
#endif //SDPPROTOCOLDATA_H
