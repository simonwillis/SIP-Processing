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

    logger = spdlog::get("stdlogger");

    this->portTotalCount = (uint16_t)((CONFIG_RTP_PORT_MAX - CONFIG_RTP_PORT_MIN) >> 1);

    portPool = new uint16_t[this->portTotalCount];
    logger->info("MediaSessionManager::MediaSessionManager setting up {} ports", this->portTotalCount);

    int index;
    uint16_t port;

    for (port = CONFIG_RTP_PORT_MIN, index = 0; (port <= CONFIG_RTP_PORT_MAX && index < this->portTotalCount); port += 2,++index) {
        this->portPool[index] = port;
    }

    this->nextReadyPortIndex = 0;
    this->nextOldPortIndex = 0;

    this->portInUseCount = 0;
    this->portAvailableCount = this->portTotalCount;

    logger->info("MediaSessionManager::MediaSessionManager created {} ports, highest port number {}", index, port);

}

void MediaSessionManager::initialise(uint16_t maxPorts) {
    portData.reserve(maxPorts);
    uint16_t port = 3999;
    for (auto i = 0; i < maxPorts; ++i) {
        if (port == 5060) {
            continue;
        }

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
    RtpPortData *pd = (RtpPortData *)handle->data;

    spdlog::get("stdlogger")->debug("RTPALOC: port {},  {} {}} bytes", pd->port, pd->active ? "ACTIVE":"INACTIVE", size);

    buf->base = (char *)malloc(size);
    memset(buf->base, 0, size);
    buf->len = size;
}

void MediaSessionManager::onUdpRecv(struct uv_udp_s *handle, long int bytes_read, const struct uv_buf_t *uv_buffer,
                                    const struct sockaddr *addr, unsigned int flags) {
    RtpPortData *pd = (RtpPortData *)handle->data;

    if (bytes_read == 0) return;

    auto me = (MediaSessionManager *) pd->me;

    spdlog::get("stdlogger")->debug("RTPDATA: port {}, {} {} bytes, flags={X}, handleFlags={X}}, type={X}}\n",
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

    this->logger->debug("MediaSessionManager::getSession looking for sessionId {}", sessionId);

    auto sessionIdSearch = this->sessions.find(sessionId);

    if (sessionIdSearch == this->sessions.end()) {
        logger->warn("ERROR: MediaSessionManager::getSession sessionId {}, no session found", sessionId);
        return nullptr;
    }

    auto session = sessionIdSearch->second;

    return session;
}

MediaSession * MediaSessionManager::getSession(std::string callId) {

    logger->debug("MediaSessionManager::getSession looking for callId {}", callId.c_str());
    auto callIdSearch =  this->callIdToSessionIdLookup.find(callId);
    if (callIdSearch == this->callIdToSessionIdLookup.end()) {
        logger->warn("ERROR: MediaSessionManager::getSession callId {}, no session found", callId.c_str());
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

    auto logger = spdlog::get("stdlogger");
    logger->debug("MediaSessionManager::allocateNewSession request for {} streams", sdpOffer.getStreamCount());
    logger->debug("MediaSessionManager::allocateNewSession Offer Received as {}", sdpOffer.toString().c_str());

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
    logger->debug("MediaSessionManager::allocate creating new offer answer for new media resource request");

    SdpOrigin origin = SdpOrigin("-", "mysessionId", 1000, CONFIG_LOCAL_IP_ADDRESS);
    sdpAnswer.setOrigin(origin);

    SdpConnection connection = SdpConnection(CONFIG_LOCAL_IP_ADDRESS);
    sdpAnswer.setConnection(connection);


    for (int i = 0; i < sdpOffer.getStreamCount(); ++i) {

        uint16_t port = this->allocatePort();

        logger->debug("MediaSessionManager::allocate Adding stream {} with port {} to offer answer", i, port);

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

    logger->debug("SDPANSWER {}", sdpAnswer.toString().c_str());

    //TODO Allocate resources and return the resourceId
    uint32_t sessionId = allocateSessionId();

    sdpAnswer.setSessionId(sessionId);

    return sessionId;
}


void MediaSessionManager::release(uint32_t sessionId) {

    this->logger->debug("MediaSessionManager::release TBC request to release session Id {}", sessionId);

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
