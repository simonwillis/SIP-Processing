//
// Created by Simon Willis on 10/06/2016.
//

#ifndef MEDIA_MEDIASTOPPLAYREQUEST_H
#define MEDIA_MEDIASTOPPLAYREQUEST_H


#include "MediaRequest.h"

class MediaPlayStopRequest : public MediaRequest {

public:
    const Method getMethod() override {
        return Method::PLAY_STOP;
    }


};


#endif //MEDIA_MEDIASTOPPLAYREQUEST_H
