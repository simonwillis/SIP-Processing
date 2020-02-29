//
// Created by Simon Willis on 13/06/2016.
//

#ifndef TESTSESSIONMANAGER_RESOURCEMANAGER_H
#define TESTSESSIONMANAGER_RESOURCEMANAGER_H


#include <list>
#include <map>
#include <atomic>
#include "Device.h"
#include "AllocatedResource.h"
#include "../definitions.h"
#include "MediaResource.h"

class ResourceManager {

public:

    ResourceManager(uint16_t deviceCount, uint16_t basePort = 3999);

    AllocatedResource * AllocateResource(media_offer_properties_t offerProperties);

    void ReleaseResource(uint32_t resourceId);

    AllocatedResource * getAllocatedResource(uint32_t resourceId);

private:

    uint32_t getNewResourceId() { return std::atomic_fetch_add(&nextResourceId, (uint32_t)1); }

    Device getDevice();

    MediaResource * createResource();

private:

    uint16_t deviceCount;
    const uint16_t basePort;
    std::list<Device> availableDevices;
    std::map<uint32_t, AllocatedResource> allocatedResources;
    std::atomic<uint32_t> nextResourceId;
};


#endif //TESTSESSIONMANAGER_RESOURCEMANAGER_H
