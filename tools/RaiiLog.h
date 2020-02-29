//
// Created by Simon Willis on 07/07/2016.
//

#ifndef UDPSEND_RAIILOG_H
#define UDPSEND_RAIILOG_H


#include <string>

#define ENABLED false

class RaiiLog {
public:
    RaiiLog(const char * message)
            : msg(message), d(++depth) {
        if (ENABLED) fprintf(stderr, "RAII OPEN: %2d %s %s\n", d, std::string(depth - 1, ' ').c_str(), msg.c_str());
    }
    ~RaiiLog() {
        --depth;
        if (depth < 0) depth = 0;

        if (ENABLED) fprintf(stderr, "RAII EXIT: %2d %s %s\n", d, std::string(depth, ' ').c_str(), msg.c_str());
    }
private:
    std::string msg;
    uint16_t d;
    static uint16_t depth;
};


#endif //UDPSEND_RAIILOG_H
