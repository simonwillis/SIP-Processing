//
// Created by Simon Willis on 08/06/2016.
//

#ifndef NOTIFICATION_CALLSTATISTICSNOTIFICATION_H
#define NOTIFICATION_CALLSTATISTICSNOTIFICATION_H

#include "Notification.hpp"

class CallStatisticsNotification : public Notification {

public:


    CallStatisticsNotification(uint32_t channelId, media_stats_t stats, int errorCode = 0, const char *errorMessage = "OK", int dialogueId = 0)
            : Notification("callMediaStatistics", channelId, errorCode, errorMessage, dialogueId)
            , stats(stats) {

    }

protected:


    bool hasData() {
        return true;
    }

    Json::Value getData() {
        Json::Value data;
        data["PaTx"] = stats.packetsTransmitted;
        data["PaRx"] = stats.packetsReceived;
        data["OORx"] = stats.packetsReceivedOutOfOrder;
        data["CLRx"] = stats.rxPacketsLost;
        data["BaRx"] = stats.badPacketsRx;
        data["DuRx"] = stats.duplicatePacketsReceived;
        data["DiRx"] = stats.discardedReceivedPackets;
        data["JiLa"] = stats.jitterLast;
        data["JiMa"] = stats.jitterMax;
        data["JiSu"] = stats.jitterSum;
        data["JiMe"] = stats.jitterMean;

        return data;
    }

private:
    media_stats_t stats;

};


#endif //NOTIFICATION_CALLSTATISTICSNOTIFICATION_H
