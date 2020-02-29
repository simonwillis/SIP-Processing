//
// Created by Simon Willis on 13/06/2016.
//

#ifndef MEDIA_MEDIARESOURCE_H
#define MEDIA_MEDIARESOURCE_H


#include "../definitions.h"
#include "Device.h"

class MediaResource {

public:

    MediaResource(Device * device)
            : device(device) {

    }

private:
    Device * device;
    media_stream_answer_t answer;
};


#endif //MEDIA_MEDIARESOURCE_H
