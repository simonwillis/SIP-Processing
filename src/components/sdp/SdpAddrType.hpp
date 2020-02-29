//
// Created by Simon Willis on 15/05/2017.
//

#ifndef ADDRTYPE_H
#define ADDRTYPE_H


class SdpAddrType {

public:

    enum Type {
        IPV4 = 1,
        IPV6 = 2
    };

public:

    SdpAddrType() { type = IPV4; }

    SdpAddrType(Type value) { type = value; }

    SdpAddrType(std::string value) { setType(value); }

    SdpAddrType(mapped_buffer_t value) { setType(value); }

    SdpAddrType(const SdpAddrType &other) { type = other.type; }

public:

    std::string toString() {
        switch (type) {
            case IPV4: return "IP4";
            case IPV6: return "IP6";
            default: return "IP4";
        }
    }
    Type getType() { return type; }

private:

    void setType(mapped_buffer_t value) {
        mapped_buffer_t buffer = buffer_trim(value);
        if (buffer_equals(buffer, "IP4")) {
            type = Type::IPV4;
        } else if (buffer_equals(buffer, "IP6")) {
            type = Type::IPV6;
        } else {
            type = IPV4;
        }
    }
    void setType(std::string value) {
        mapped_buffer_t buffer = mapped_buffer(value);
        setType(buffer);
    }
private:
    Type type = IPV4;
};


#endif //ADDRTYPE_H
