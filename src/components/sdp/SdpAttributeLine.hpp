//
// Created by simon on 12/05/17.
//

#ifndef UDPSEND_SDPATTRIBUTE_HPP
#define UDPSEND_SDPATTRIBUTE_HPP

#include <sstream>
#include "../utilities/buffers.h"

class SdpAttributeLine {

public:

    SdpAttributeLine() {}

    SdpAttributeLine(mapped_buffer_t line) {
        std::vector<mapped_buffer_t> components = buffer_split_on_first(buffer_trim(line), ' ');
        std::vector<mapped_buffer_t> kvp = buffer_split_on_first(components[0], ':');
        isPropertyAttribute = (kvp[1].len == 0);
        key = bufferToString(buffer_trim(kvp[0]));
        if (! isPropertyAttribute) {
            value = bufferToString(buffer_trim(kvp[1]));
        }
        if (components.size() > 1) {
            parameters = bufferToString(components[1]);
        }
    }

    SdpAttributeLine(const SdpAttributeLine &other) : isPropertyAttribute(other.isPropertyAttribute), key(other.key), value(other.value), parameters(other.parameters) { }

public:

    std::string toString() {

        std::stringstream ss;
        ss << key;

        if (! isPropertyAttribute) {
            ss << ':' << value;
        }

        if (! parameters.empty()) {
            ss << ' ' << parameters;
        }

        return ss.str();
    }

public:

    bool isPropertyAttribute;
    std::string key;
    std::string value = std::string();
    std::string parameters = std::string();

};


#endif //UDPSEND_SDPATTRIBUTE_HPP
