//
// Created by Simon Willis on 13/06/2016.
//

#include "AllocatedResource.h"

AllocatedResource::AllocatedResource(uint32_t resourceId)
        : resourceId(resourceId) {

    for (size_t i = 0; i < Max_Media_Channels; ++i) {
        resources[i] = NULL;
    }

}
