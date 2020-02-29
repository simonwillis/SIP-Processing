//
// Created by simon on 21/05/17.
//

#ifndef RTPPACKET_HPP
#define RTPPACKET_HPP


#include <cstdint>

class RtpPacket {
public:
    RtpPacket(uint8_t * raw) : raw(raw) {}
    uint16_t header;     // check
    uint16_t sequence;   // check
    uint32_t timestamp;  // check
    uint32_t srsc;       // check
private:
    uint8_t * raw;
};

#endif //RTPPACKET_HPP
