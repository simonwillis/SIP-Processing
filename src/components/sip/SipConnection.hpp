//
// Created by Simon Willis on 08/07/2016.
//

#ifndef UDPSEND_SIPCONNECTION_H
#define UDPSEND_SIPCONNECTION_H


#include <uv.h>
#include "../utilities/MessageBuffer.h"
#include "SipParser.h"


class SipConnection {

public:

    SipConnection(uint32_t connId, void * userData, uv_stream_t * uvStream, SipMessageEventHandler onSipEvent)
            : connectionId(connId), data(userData), stream(uvStream) {

        messageBuffer = new MessageBuffer(16192);
        sipParser = new SipParser(onSipEvent, this);
        pendingMessageBytes = 0;
    };

    ~SipConnection() {
        if (messageBuffer) {
            delete messageBuffer;
        }
    }

public:

    uint32_t connectionId;
    void * data;
    MessageBuffer * messageBuffer;
    uv_stream_t * stream;
    SipParser * sipParser;
    size_t pendingMessageBytes;

};


#endif //UDPSEND_SIPCONNECTION_H
