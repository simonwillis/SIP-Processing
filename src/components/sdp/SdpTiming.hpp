//
// Created by simon on 12/05/17.
//

#ifndef SDPTIMING_H
#define SDPTIMING_H

#include <sstream>
#include "../utilities/buffers.h"
#include "../media/definitions.h"

class SdpTiming {

public:

    SdpTiming() {}

    SdpTiming(mapped_buffer_t rawTLineData) {
        std::vector<mapped_buffer_t> content = buffer_split(rawTLineData, ' ');
        if (content.size() >= 2) {
            start = toLongInteger(content[0]);
            stop = toLongInteger(content[1]);
        }
    }

    SdpTiming(const SdpTiming &other)
            : start(other.start), stop(other.stop) { }

public:

    std::string toString() {
        std::stringstream ss;
        ss << "t=";
        ss << start << ' ' << stop;
        return ss.str();
    }

    std::string toJson() {
        std::stringstream ss;
        ss << "{\"start\": \"" << start << "\", \"end\": \"" << stop << "\"}";
        return ss.str();
    }

public:
    uint32_t start = 0;
    uint32_t stop = 0;
};
#endif //SDPTIMING_H
