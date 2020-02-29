//
// Created by Simon Willis on 15/05/2017.
//

#ifndef SDPMEDIADATA_H
#define SDPMEDIADATA_H


#include "../utilities/buffers.h"

class SdpMediaType {
public:
    enum Type {
        unknown,
        audio,
        video,
        text,
        application,
        message
    };

public:

    SdpMediaType() : type(audio) { }

    SdpMediaType(Type value) : type(value) { }

    SdpMediaType(std::string value) { setType(value); }

    SdpMediaType(mapped_buffer_t value) { setType(value); }

    SdpMediaType(const SdpMediaType &other) : type(other.type) { }

public:

    std::string toString() {
        switch (type) {
            case audio: return "audio";
            case video: return "video";
            case text: return "text";
            case application: return "application";
            case message: return "message";
            default: return "unknown";
        }
    }

    Type getType() { return type; }

private:

    void setType(mapped_buffer_t value) {
        mapped_buffer_t buffer = buffer_trim(value);
        if (buffer_equals(buffer, "audio")) {
            type = Type::audio;
        } else if (buffer_equals(buffer, "video")) {
            type = Type::video;
        } else if (buffer_equals(buffer, "text")) {
            type = SdpMediaType::text;
        } else if (buffer_equals(buffer, "application")) {
            type = application;
        } else if (buffer_equals(buffer, "message")) {
            type = message;
        } else {
            type = unknown;
        }
    }

    void setType(std::string value) {
        mapped_buffer_t buffer = mapped_buffer(value);
        setType(buffer);
    }

private:
    Type type = audio;

};

#endif //SDPMEDIADATA_H
