//
// Created by Simon Willis on 12/06/2016.
//

#ifndef MEDIA_DEFINITIONS_H
#define MEDIA_DEFINITIONS_H

#include <string>
#include <vector>

#include "../utilities/buffers.h"

static constexpr size_t MaxSdpBodyLength = 4096;
static constexpr uint8_t MediaIndexMediaType = 0;
static constexpr uint8_t MediaIndexTransportPort = 1;
static constexpr uint8_t MediaIndexTransportProtocol = 2;
static constexpr uint8_t MediaIndexFirstFormatItem = 3;
static constexpr uint8_t OriginIndexUsername = 0;
static constexpr uint8_t OriginIndexSessionId = 1;
static constexpr uint8_t OriginIndexSessionVersion = 2;
static constexpr uint8_t OriginIndexNetType = 3;
static constexpr uint8_t OriginIndexAddressType = 4;
static constexpr uint8_t OriginIndexAddress = 5;
static constexpr uint8_t ConnectionIndexNetType = 0;
static constexpr uint8_t ConnectionIndexAddressType = 1;
static constexpr uint8_t ConnectionIndexAddress = 2;
static constexpr uint8_t TimingIndexStart = 0;
static constexpr uint8_t TimingIndexStop = 1;



typedef struct {

    std::vector<uint16_t> formats;
    std::string remoteAddress;
    std::string direction; // "sendrecv" | "sendonly" | "recvonly"
    uint16_t remotePort;

} media_stream_offer_t;

typedef struct {

    uint16_t format;
    std::string localAddress;
    std::string direction; // "sendrecv" | "sendonly" | "recvonly"
    uint16_t localPort;

} media_stream_answer_t;

typedef struct {

    uint8_t streamCount;
    uint8_t something;
    uint16_t mediaType;
    std::vector<media_stream_offer_t> streamData;

} media_offer_properties_t;

typedef struct {

    uint8_t streamCount;
    uint8_t something;
    uint16_t mediaType;
    std::vector<media_stream_answer_t> streamData;

} media_answer_properties_t;


#endif //MEDIA_DEFINITIONS_H
