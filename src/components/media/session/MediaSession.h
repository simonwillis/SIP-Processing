//
// Created by Simon Willis on 23/05/2017.
//

#ifndef MEDIASESSION_H
#define MEDIASESSION_H


#include <atomic>
#include <uv.h>

#include "../../sdp/Sdp.h"

class MediaSession {

public:

    enum DataType {
        Media = 0,
        Control = 1
    };
    struct PortLocationData {
        uint32_t sessionId;
        uint8_t streamIndex;
        DataType dataType;
    };

public:

    MediaSession() {

    }

    MediaSession(Sdp & offer) {

    }

    MediaSession(const MediaSession &other) {

    }

public:

    ~MediaSession() { }

public:

    void setSdpResponse(Sdp & sdpResponse) {
        this->sdpResponse = sdpResponse;
    }

    Sdp * getResponse() {
        return &sdpResponse;
    }

    uint32_t getId() { return sessionId; }

    void startReceive() {


    }

private:
    struct Ports {
        uint16_t rtpPort;
        uv_udp_t rtpReceiver;
        uint16_t rtcpPort = 0;
        uv_udp_t rtcpReceiver;
    };
    struct Address {
        std::string address;
        Ports ports;
    };
    struct Stream {
        Address remoteAddress;
        Ports localPorts;
    };

private:
    uint32_t sessionId;
    Sdp sdpOffer;
    Sdp sdpResponse;

    std::vector<Stream> streams;

    Address remoteAddress;

    // TODO Set up new Session based on the SDP offer
    //
    // channel count
    // remote address
    // remote ports
    //
    // resourceId - reference back to the allocated session data
    // sessionId
    // local address
    // local ports


};


#endif //MEDIASESSION_H
