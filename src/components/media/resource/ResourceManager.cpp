//
// Created by Simon Willis on 13/06/2016.
//

#include "ResourceManager.h"


ResourceManager::ResourceManager(uint16_t deviceCount, uint16_t basePort)
        : deviceCount(deviceCount), basePort(basePort) {

    fprintf(stderr, "ResourceManager::CTOR [deviceCount=%u] [basePort=%u]\n", deviceCount, basePort);

    for (uint16_t i = 0; i < deviceCount; ++i) {

        availableDevices.push_back(Device(basePort + (i * 2)));
    }

}

AllocatedResource *  ResourceManager::AllocateResource(media_offer_properties_t offerProperties) {

    AllocatedResource * allocatedResource = nullptr;

    fprintf(stderr, "ResourceManager::AllocateResource received offer properties\n");

    uint32_t resourceId = getNewResourceId();

    fprintf(stderr, "ResourceManager::AllocateResource given id [resourceId=%u]\n", resourceId);



    return allocatedResource;

}


void ResourceManager::ReleaseResource(uint32_t resourceId) {

}


AllocatedResource * ResourceManager::getAllocatedResource(uint32_t resourceId) {

    return NULL;
}