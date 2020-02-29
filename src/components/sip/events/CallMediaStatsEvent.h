//
// Created by Simon Willis on 24/05/2016.
//

#ifndef TESTSIPPARSING_CALLMEDIASTATSEVENT_H
#define TESTSIPPARSING_CALLMEDIASTATSEVENT_H


#include <list>
#include "CallEvent.h"
#include "../SipInterface.h"

class CallMediaStatsEvent : public CallEvent {

public:

    CallMediaStatsEvent(uint32_t channelId, std::string callId, media_stats_t stats)
            : CallEvent(channelId, callId), stats(stats) {

    }

    virtual CallEventType getEventType() override {
        return CallEventType::MEDIA_STATS;
    }

    virtual const char *getEventTypeName() override {
        return "Media Statistics";
    }

    media_stats_t getStats() {
        return stats;
    }

    Json::Value getData() {
        Json::Value data;
        Json::Value values;
        values["PaTx"] = stats.packetsTransmitted;
        values["PaRx"] = stats.packetsReceived;
        values["OORx"] = stats.packetsReceivedOutOfOrder;
        values["CLRx"] = stats.rxPacketsLost;
        values["BaRx"] = stats.badPacketsRx;
        values["DuRx"] = stats.duplicatePacketsReceived;
        values["DiRx"] = stats.discardedReceivedPackets;
        values["JiLa"] = stats.jitterLast;
        values["JiMa"] = stats.jitterMax;
        values["JiSu"] = stats.jitterSum;
        values["JiMe"] = stats.jitterMean;

        data["stats"] = values;

        return data;
    }


private:
    media_stats_t stats;

};


#endif //TESTSIPPARSING_CALLMEDIASTATSEVENT_H
