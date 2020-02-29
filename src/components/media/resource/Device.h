//
// Created by Simon Willis on 13/06/2016.
//

#ifndef MEDIA_DEVICE_H
#define MEDIA_DEVICE_H

#include <string>

class Device {

public:

    Device(uint16_t port)
            : port(port) {
    }

    const uint16_t port;

    uint32_t allocatedResourceId;

};


#endif //MEDIA_DEVICE_H
