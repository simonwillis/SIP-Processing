//
// Created by Simon Willis on 23/05/2017.
//

#ifndef RTCPPACKET_H
#define RTCPPACKET_H

#include <cstdint>

class RtcpPacket {
public:
    RtcpPacket(uint8_t * raw) : raw(raw) {}
    uint16_t header;     // check
    uint16_t sequence;   // check
    uint32_t timestamp;  // check
    uint32_t srsc;       // check
private:
    uint8_t * raw;
};



#endif //RTCPPACKET_H
