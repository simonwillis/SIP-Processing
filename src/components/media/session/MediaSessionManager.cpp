//
// Created by Simon Willis on 23/05/2017.
//

#include "MediaSessionManager.h"

#define CONFIG_LOCAL_IP_ADDRESS "192.168.0.4"
#define CONFIG_RTP_PORT_MIN 4000
#define CONFIG_RTP_PORT_MAX 6998

//uint32_t MediaSessionManager::nextSessionId = 1001;
std::atomic<uint32_t> MediaSessionManager::nextSessionId(1001);

MediaSessionManager::MediaSessionManager() {

    this->portTotalCount = (uint16_t)((CONFIG_RTP_PORT_MAX - CONFIG_RTP_PORT_MIN) >> 1);

    portPool = new uint16_t[this->portTotalCount];
    fprintf(stderr, "MediaSessionManager::MediaSessionManager setting up %u ports\n", this->portTotalCount);

    int index;
    uint16_t port;

    for (port = CONFIG_RTP_PORT_MIN, index = 0; (port <= CONFIG_RTP_PORT_MAX && index < this->portTotalCount); port += 2,++index) {
        this->portPool[index] = port;
    }

    this->nextReadyPortIndex = 0;
    this->nextOldPortIndex = 0;

    this->portInUseCount = 0;
    this->portAvailableCount = this->portTotalCount;

    fprintf(stderr, "MediaSessionManager::MediaSessionManager created %d ports, highest port number %u\n", index, port);

}

void MediaSessionManager::initialise(uint16_t maxPorts) {
    portData.reserve(maxPorts);
    uint16_t port = 3999;
    for (auto i = 0; i < maxPorts; ++i) {
        if (port == 5060) {
            continue;
        }

        fprintf(stderr, "Setting up port index %u, => %u\n", i, port);
        uv_udp_init(uv_default_loop(), &portData[i].socket);

        portData[i].socket.data = (void *) &portData[i];
        portData[i].port = port;
        portData[i].me = this;
        portData[i].active = false;

        struct sockaddr_in udp_recv_addr;
        uv_ip4_addr("0.0.0.0", port, &udp_recv_addr);
        uv_udp_bind(&portData[i].socket, (const struct sockaddr *)&udp_recv_addr, 0);
        uv_udp_recv_start(&portData[i].socket, allocateUdpDataBuffer, onUdpRecv);

        ++port;
    }
}

void MediaSessionManager::allocateUdpDataBuffer(struct uv_handle_s *handle, long unsigned int size,
                                                struct uv_buf_t *buf) {
    //RtpPortData *pd = (RtpPortData *)handle->data;

    //fprintf(stderr, "RTPALOC: port %d,  %s %d bytes\n", pd->port, pd->active ? "ACTIVE":"INACTIVE", size);

    buf->base = (char *)malloc(size);
    memset(buf->base, 0, size);
    buf->len = size;
}

void MediaSessionManager::onUdpRecv(struct uv_udp_s *handle, long int bytes_read, const struct uv_buf_t *uv_buffer,
                                    const struct sockaddr *addr, unsigned int flags) {
    RtpPortData *pd = (RtpPortData *)handle->data;

    if (bytes_read == 0) return;

    auto me = (MediaSessionManager *) pd->me;

    //fprintf(stderr, "RTPDATA: flags = 0X%X, handle flags=0X%X, type = 0x%X\n", flags, handle->flags, handle->type);
    fprintf(stderr, "RTPDATA: port %d, %s %d bytes, flags=0x%X, handleFlags=0x%X, type=0x%X\n",
            pd->port, pd->active ? "ACTIVE":"INACTIVE", bytes_read, flags, handle->flags, handle->type);

    if (bytes_read > 0) {
        if (pd->port & 0x01) {
            // It is an odd numbered port. For now assume that this is RTCP
            me->showRtcpHeader((void *) uv_buffer->base, bytes_read);
        } else {
            me->showRtpHeader((void *) uv_buffer->base, bytes_read);
        }
    }

    free(uv_buffer->base);

}

uint16_t MediaSessionManager::allocatePort() {
    uint16_t port = this->portPool[this->nextReadyPortIndex++];
    if (this->nextReadyPortIndex >= this->portTotalCount) {
        this->nextReadyPortIndex = 0;
    }
    return port;
}

uint32_t MediaSessionManager::allocateSessionId() {
    return MediaSessionManager::nextSessionId++;
}

void MediaSessionManager::releasePort(uint16_t port) {
    if (this->nextOldPortIndex >= this->portTotalCount) {
        this->nextOldPortIndex = 0;
    }
    this->portPool[this->nextOldPortIndex] = port;
}

MediaSession * MediaSessionManager::getSession(uint32_t sessionId) {

    fprintf(stderr, "MediaSessionManager::getSession looking for sessionId %u\n", sessionId);

    auto sessionIdSearch = this->sessions.find(sessionId);

    if (sessionIdSearch == this->sessions.end()) {
        fprintf(stderr, "ERROR: MediaSessionManager::getSession sessionId=%u, no session found\n", sessionId);
        return nullptr;
    }

    auto session = sessionIdSearch->second;

    return session;
}

MediaSession * MediaSessionManager::getSession(std::string callId) {

    fprintf(stderr, "MediaSessionManager::getSession looking for callId '%s'\n", callId.c_str());
    auto callIdSearch =  this->callIdToSessionIdLookup.find(callId);
    if (callIdSearch == this->callIdToSessionIdLookup.end()) {
        fprintf(stderr, "ERROR: MediaSessionManager::getSession callId=%s, no session found\n", callId.c_str());
        return nullptr;
    }
    auto sessionId = callIdSearch->second;

    return getSession(sessionId);
}

MediaSession MediaSessionManager::retrieveSession(uint32_t sessionId) {
    // All public calls to MediaSessionManager are protected using a blunt whole call RAII scoped approach
    std::lock_guard<std::mutex> guard(mutex);

    MediaSession result;
    auto session = getSession(sessionId);

    if (session) {
        result = MediaSession(*session);
    } else {
        result = MediaSession();
    }

    return result;
}

uint32_t MediaSessionManager::allocate(std::string callId, Sdp &sdpOffer, Sdp &sdpAnswer) {
    fprintf(stderr, "MediaSessionManager::allocateNewSession request for %u streams\n", sdpOffer.getStreamCount());
    fprintf(stderr, "MediaSessionManager::allocateNewSession Offer Received as\n%s\n", sdpOffer.toString().c_str());

    /*
     * LOCK ENTIRE PROCESS - use lock_guard for RAII simplicity
     *
     * Check if we already have this call Id in progress - Re-Invite
     *
     * Check what codecs should be used
     *
     * Check/Get resources are available for codec
     *
     * Create answer based on response above
     *
     * Create new Media Session - referenced off CallID ?
     *
     * Add media session to collection
     *
     * Return id
     *
     */

     // All public calls to MediaSessionManager are protected using a blunt whole call RAII scoped approach
    std::lock_guard<std::mutex> guard(mutex);

    auto session = this->getSession(callId);

    if (session) {
        return session->getId();
    }

    // This is new call offer
    fprintf(stderr, "MediaSessionManager::allocate creating new offer answer for new media resource request\n");


    SdpOrigin origin = SdpOrigin("-", "mysessionId", 1000, CONFIG_LOCAL_IP_ADDRESS);
    sdpAnswer.setOrigin(origin);

    SdpConnection connection = SdpConnection(CONFIG_LOCAL_IP_ADDRESS);
    sdpAnswer.setConnection(connection);


    for (int i = 0; i < sdpOffer.getStreamCount(); ++i) {

        uint16_t port = this->allocatePort();

        fprintf(stderr, "MediaSessionManager::allocate Adding stream %ul with port %u to offer answer\n", i, port);

        SdpMedia m(SdpMediaType::audio, port, SdpProtocolType::rtp);

        m.addFormat(SdpFormatType::PCMU);
        m.addAttribute("rtpmap", "0", "PCMU/8000");


        m.addFormat(SdpFormatType::DTMF);
        m.addAttribute("rtpmap", "101", "telephone-event/8000");
        m.addAttribute("fmtp", "101", "0-15");

        m.addAttribute("ptime", "20");


//        SdpAttribute directionAttribute = SdpAttribute("sendrecv");
//        m.addAttribute(directionAttribute);

        sdpAnswer.addMediaStream(m);


    }


    fprintf(stderr, "SDPANSWER\n%s", sdpAnswer.toString().c_str());


    //TODO Allocate resources and return the resourceId
    uint32_t sessionId = allocateSessionId();


    sdpAnswer.setSessionId(sessionId);

    return sessionId;
}


void MediaSessionManager::release(uint32_t sessionId) {
    fprintf(stderr, "MediaSessionManager::release TBC request to release session Id %u\n", sessionId);

    std::lock_guard<std::mutex> guard(mutex);

    auto session = getSession(sessionId);

    if (session) {

    }

    return;
}

/*
 *
 * PRIVATE MEMBER FUNCTIONS
 *
 */

//uint16_t MediaSessionManager::allocatePort() {
//
//}
//
//void MediaSessionManager::releasePort() {
//
//}
