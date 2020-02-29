//
// Created by Simon Willis on 10/06/2016.
//

#ifndef MEDIA_MEDIASTARTPLAYREQUEST_H
#define MEDIA_MEDIASTARTPLAYREQUEST_H


#include "MediaRequest.h"

class MediaPlayStartRequest : public MediaRequest {

public:
    const Method getMethod() override {
        return Method::PLAY_START;
    }

};


#endif //MEDIA_MEDIASTARTPLAYREQUEST_H
