//
// Created by simon on 16/05/17.
//

#ifndef SDPFORMAT_HPP
#define SDPFORMAT_HPP

#include "SdpFormatType.hpp"

class SdpFormat {

public:

    enum FormatDescriptorType {
        PayloadFormat,
        FormatParameter
    };

    SdpFormat() { }

    SdpFormat(uint8_t id, FormatDescriptorType type = PayloadFormat)
            : id(id), type(type) { }

public:
    static const char * getFormatName(uint8_t formatId) {
        switch (formatId) {
            case 0: return "PCMU";
            case 8: return "PCMA";
            case 9: return "G722";
            default: return "UNKNOWN";
        }
    }

public:

    uint16_t getFmt() { return id; }

    std::string toString() {
        std::stringstream ss;
        ss << "a=";
        ss << ((type == PayloadFormat) ? "rtpmap:" : "fmtp");
        ss << ' ';
        ss << getFormatName(id) << "/8000";
        return ss.str();
    }

private:

    uint8_t id;
    FormatDescriptorType type;

};

#endif //SDPFORMAT_HPP
