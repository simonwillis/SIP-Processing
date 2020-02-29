//
// Created by Simon Willis on 13/06/2016.
//

#ifndef MEDIA_ALLOCATEDRESOURCE_H
#define MEDIA_ALLOCATEDRESOURCE_H


#include "../definitions.h"
#include "MediaResource.h"

#define Max_Media_Channels 2


class AllocatedResource {

public:

    AllocatedResource(uint32_t resourceId);



private:

    uint32_t resourceId;
    uint8_t deviceCount;
    MediaResource * resources[Max_Media_Channels];
    std::string recordFilename;
    bool isRecording;


};


#endif //MEDIA_ALLOCATEDRESOURCE_H
