//
// Created by Simon Willis on 03/06/2016.
//

#ifndef SIP_MESSAGES__SIPMESSAGEBODY_H
#define SIP_MESSAGES__SIPMESSAGEBODY_H


#include <string>
#include "../../utilities/buffers.h"

class SipMessageBody {

public:
    SipMessageBody(mapped_buffer_t message) {

    }
    enum class BodyType {
        UNKNOWN = 0x00,
        NONE,
        SDP,
        XML,
        MULTIPLE,
        BOUNDARY,
        OTHER
    };

public:
    static const char * toString(BodyType bodyType) {
        switch (bodyType) {
            case BodyType::UNKNOWN: return "UNKNOWN";
            case BodyType::MULTIPLE : return "MULTIPLE";
            case BodyType::XML : return "XML";
            case BodyType::SDP : return "SDP";
            case BodyType::BOUNDARY : return "BOUNDARY";
            case BodyType::OTHER : return "OTHER";

            case BodyType::NONE :
            default:
                return "NONE";
        }
    }

public:
    BodyType bodyType;
    mapped_buffer_t contentType;
    mapped_buffer_t contentDisposition;
    mapped_buffer_t body;
};


#endif //SIP_MESSAGES_SIPMESSAGEBODY_H
