//
// Created by Simon Willis on 13/06/2016.
//

#include "ResourceManager.h"


ResourceManager::ResourceManager(uint16_t deviceCount, uint16_t basePort)
        : deviceCount(deviceCount), basePort(basePort) {

    logger = spdlog::get("stdlogger");

    logger->info("ResourceManager created deviceCount={}, basePort={}", deviceCount, basePort);

    for (uint16_t i = 0; i < deviceCount; ++i) {

        availableDevices.push_back(Device(basePort + (i * 2)));
    }

}

AllocatedResource *  ResourceManager::AllocateResource(media_offer_properties_t offerProperties) {

    AllocatedResource * allocatedResource = nullptr;

    logger->debug("ResourceManager::AllocateResource received offer properties\n");

    uint32_t resourceId = getNewResourceId();

    logger->debug("ResourceManager::AllocateResource given id [resourceId=%u]\n", resourceId);

    return allocatedResource;
}


void ResourceManager::ReleaseResource(uint32_t resourceId) {
    //TODO Implement ResourceManager::ReleaseResource
}


AllocatedResource * ResourceManager::getAllocatedResource(uint32_t resourceId) {
    //TODO Implement ResourceManager::getAllocatedResource
    return NULL;
}