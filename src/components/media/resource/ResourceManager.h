//
// Created by Simon Willis on 13/06/2016.
//

#ifndef MEDIA_RESOURCEMANAGER_H
#define MEDIA_RESOURCEMANAGER_H


#include <list>
#include <map>
#include <atomic>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
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

    std::shared_ptr<spdlog::logger> logger;
};


#endif //MEDIA_RESOURCEMANAGER_H
