//
// Created by simon on 12/05/17.
//

#ifndef SDPSESSION_H
#define SDPSESSION_H

#include <sstream>
#include "../utilities/buffers.h"
#include "../media/definitions.h"

class SdpSession {

public:

    SdpSession() : name("-") {}

    SdpSession(mapped_buffer_t rawSLineData) {
        name = bufferToString(rawSLineData);
    }

    SdpSession(const SdpSession &other)
            : name(other.name) { }

public:

    std::string toString() {
        std::stringstream ss;
        ss << "s=";
        ss << name;
        return ss.str();
    }

    std::string toJson() {
        std::stringstream ss;
        ss << "{\"name\":\"" << name << "\"}";
        return ss.str();
    }

private:
    std::string name;
};
#endif //SDPSESSION_H


