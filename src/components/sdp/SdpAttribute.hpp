//
// Created by simon on 16/05/17.
//

#ifndef SDPATTRIBUTE_HPP
#define SDPATTRIBUTE_HPP

#include "SdpAttributeLine.hpp"

class SdpAttribute {

public:

    SdpAttribute() { }

    SdpAttribute(SdpAttributeLine line) : key(line.key), value(line.value), parameters(line.parameters), isPropertyAttribute(line.isPropertyAttribute) { }

    SdpAttribute(const char * key) : key(key), value(""), parameters(""), isPropertyAttribute(true) {}

    SdpAttribute(const char * key, const char * value) : key(key), value(value), parameters(""), isPropertyAttribute(value ? false:true) {}

    SdpAttribute(const char * key, const char * value, const char * parameters) : key(key), value(value), parameters(parameters ? parameters: ""), isPropertyAttribute(value ? false:true) {}

    SdpAttribute(std::string &key) : key(key), value(""), parameters(""), isPropertyAttribute(true) {}

    SdpAttribute(std::string &key, std::string &value, std::string &parameters) : key(key), value(value), parameters(parameters), isPropertyAttribute(value.empty()) {}

    SdpAttribute(const SdpAttribute &other) : key(other.key), value(other.value), parameters(other.parameters), isPropertyAttribute(other.isPropertyAttribute) { }

public:

    std::string toString() {
        std::stringstream ss;
        ss << "a=" << key;
        if (! isPropertyAttribute) {
            ss << ':' << value;
        }
        if (! parameters.empty()) {
            ss << ' ' << parameters;
        }

        return ss.str();
    }

    std::string toJson() {
        std::stringstream ss;
        ss << "{";
        ss << "\"name\":\"" << key << "\",";
        ss << "\"value\":\"" << value << "\",";
        ss << "\"parameters\":\"" << parameters << "\"";
        ss << "}";

        return ss.str();
    }

private:

    std::string key;
    std::string value;
    std::string parameters;
    bool isPropertyAttribute;

};

#endif //SDPATTRIBUTE_HPP
