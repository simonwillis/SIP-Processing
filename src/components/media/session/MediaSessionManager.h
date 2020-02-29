//
// Created by Simon Willis on 23/05/2017.
//

#ifndef MEDIASESSIONMANAGER_H
#define MEDIASESSIONMANAGER_H


#include <cstdint>
#include <mutex>
#include "../../sdp/Sdp.h"
#include "MediaSession.h"

class MediaSessionManager {

public:

    MediaSessionManager();

public:

    ~MediaSessionManager() { }

public:

    void initialise(uint16_t maxPorts);

    uint32_t allocate(std::string callId, Sdp &sdpOffer, Sdp &sdpAnswer);

    void release(uint32_t id);

    MediaSession retrieveSession(uint32_t id);

private:

    struct RtpPortData {
        MediaSessionManager * me;
        uint16_t port;
        uv_udp_t socket;
        bool active;
    };

#pragma pack(push, 1)
    struct RtpHeader {
        uint8_t byte1;
        uint8_t type;
        uint8_t seq[2];
        uint8_t ts[4];
        uint32_t ssrc;
        uint32_t csrc[16];
    };
    struct RtpDumb {
        uint8_t bytes[8];
    };

#pragma pack(pop)

    std::mutex mutex;

    void showRtpHeader(void * data, long int size) {
        RtpHeader * h = (RtpHeader *) data;


        fprintf(stderr, "RTP Header (size=%u bytes) ", size);
        fprintf(stderr, " Version: %u\n", (h->byte1 & 0b11000000) >> 6);
        fprintf(stderr, " Padding: %s\n", (h->byte1 & 0b00100000) ? "true":"false");
        fprintf(stderr, " Extension: %s\n", (h->byte1 & 0b00010000) ? "true":"false");
        fprintf(stderr, " SourceIds: %u\n", (h->byte1 & 0b00001111));
        fprintf(stderr, " Marker: %s\n", (h->type & 0x80 ) ? "true":"false");
        fprintf(stderr, " Type: %u  [%02X]\n ", (h->type & 0x7F), h->type);
        fprintf(stderr, " Sequence: %u  [%02X, %02x]\n", (h->seq[1] + (h->seq[0] << 8) ), h->seq[0], h->seq[1]);
        fprintf(stderr, " Timestamp: %u  [%02X, %02X, %02X, %02X]\n", (h->ts[3] + (h->ts[2] << 8) + (h->ts[1] << 16) + (h->ts[0] << 24) ), h->ts[0], h->ts[1], h->ts[2], h->ts[3] );

        long int max = size;
        unsigned char * p = (unsigned char *)data;
        for (auto i = 0; i < max; i++) {
            if (i == 44) {
                fprintf(stderr, " - ");
            }
            fprintf(stderr, " %02X", *(p++));
        }
        fprintf(stderr, "\n\n");
    }

    void showRtcpHeader(void * data, long int size) {
        RtpHeader * h = (RtpHeader *) data;


        fprintf(stderr, "RTP Header (size=%u bytes) ", size);
        fprintf(stderr, " Version: %u\n", (h->byte1 & 0b11000000) >> 6);
        fprintf(stderr, " Padding: %s\n", (h->byte1 & 0b00100000) ? "true":"false");
        fprintf(stderr, " Extension: %s\n", (h->byte1 & 0b00010000) ? "true":"false");
        fprintf(stderr, " SourceIds: %u\n", (h->byte1 & 0b00001111));
        fprintf(stderr, " Type: %u  [%02X]\n ", h->type, h->type);

        long int max = size;
        unsigned char * p = (unsigned char *)data;
        for (auto i = 0; i < max; i++) {
            if (i == 44) {
                fprintf(stderr, " - ");
            }
            fprintf(stderr, " %02X", *(p++));
        }
        fprintf(stderr, "\n\n");

    }


    static void allocateUdpDataBuffer(struct uv_handle_s *handle, long unsigned int size, struct uv_buf_t *buf);
    static void onUdpRecv(struct uv_udp_s *handle, long int bytes_read, const struct uv_buf_t *uv_buffer,
                          const struct sockaddr *addr, unsigned int flags);

    uint16_t * portPool;
    uint16_t nextReadyPortIndex;
    uint16_t nextOldPortIndex;
    uint16_t portAvailableCount;
    uint16_t portInUseCount;
    uint16_t portTotalCount;

    std::map<std::string, uint32_t> callIdToSessionIdLookup = std::map<std::string, uint32_t>();
    std::map<uint32_t, MediaSession *> sessions = std::map<uint32_t, MediaSession *>();

    uint16_t allocatePort();

    MediaSession * getSession(uint32_t id);
    MediaSession * getSession(std::string callId);
    uint32_t allocateSessionId();

    void releasePort(uint16_t port);

    std::vector<RtpPortData> portData;


    //static uint32_t nextSessionId;
    static std::atomic<uint32_t> nextSessionId;
};


#endif //MEDIASESSIONMANAGER_H
