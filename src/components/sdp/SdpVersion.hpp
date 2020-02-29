//
// Created by simon on 12/05/17.
//

#ifndef SDPVERSION_H
#define SDPVERSION_H

#include <sstream>
#include "../utilities/buffers.h"
#include "../media/definitions.h"

class SdpVersion {

public:

    SdpVersion() : version("0") {}

    SdpVersion(mapped_buffer_t rawLineData) {
        version = bufferToString(rawLineData);
    }

    SdpVersion(const SdpVersion &other)
            : version(other.version) { }

public:

    std::string toString() {
        std::stringstream ss;
        ss << "v=" << version;
        return ss.str();
    }

    std::string toJson() {
        std::stringstream ss;
        ss << "{\"version\":\"" << version << "\"}";
        return ss.str();
    }

private:
    std::string version;
};
#endif //SDPVERSION_H


