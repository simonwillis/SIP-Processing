//
// Created by Simon Willis on 09/06/2016.
//

#include <iostream>

#include <spdlog/spdlog.h>

#include "../components/media/MediaServerInterface.h"
#include "../components/media/messages/MediaRequest.h"
#include "../components/media/messages/MediaRegisterResponse.h"
#include "../components/media/messages/MediaAllocateResponse.h"
#include "../components/media/definitions.h"
#include "../components/media/messages/MediaAllocateRequest.h"
#include "../components/media/messages/MediaReleaseRequest.h"
#include "../components/media/messages/MediaReleaseResponse.h"
#include "../components/media/messages/MediaRecordStartRequest.h"
#include "../components/media/messages/MediaRecordStartResponse.h"
#include "../components/media/messages/MediaRecordStopRequest.h"
#include "../components/media/messages/MediaRecordStopResponse.h"

#include "../components/sdp/Sdp.h"
#include "../components/sdp/SdpOrigin.hpp"
#include "../components/media/session/MediaSessionManager.h"

#include <uv.h>

MediaServerInterface * mediaServerInterface;
MediaSessionManager * mediaSessionManager;


void on_ms20_timer(uv_timer_t * timer) {

    long key = (long)timer->data;
    //fprintf(stderr, "on_timer id=%ld\n", key);
}

void loop_process(void * data) {

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void on_connect(uv_stream_t * server, int status) {
    fprintf(stderr, "on_connect\n");

}

void mediaRequestHandler(MediaRequest * request, uv_stream_t * stream) {
    fprintf(stderr, "Media request received \n");

    if (! request) {
        fprintf(stderr, "media::mediaRequestHandler passed a NULL request object\n");
        return;
    }

    if (! stream) {
        fprintf(stderr, "media::mediaRequestHandler passed a NULL stream object\n");
        return;
    }

    fprintf(stderr, "Media request received %s:%s:%s[id=%d]\n", request->getDomainStr(), request->getMethodStr(), request->getTypeStr(), request->getDialogueId());

    MediaMessage::Method method = request->getMethod();

    fprintf(stderr, "Media request received. Method=%u\n", method);

    switch (method) {

        case MediaMessage::Method::UNKNOWN:
            break;

        case MediaMessage::Method::REGISTER: {
            MediaRegisterResponse response(request->getDialogueId(), request->getCallbackId(), 0, "OK");

            mediaServerInterface->PostResponse(stream, response);
        }
        break;

        case MediaMessage::Method::ALLOCATE: {
            auto * req = (MediaAllocateRequest *) request;

            std::string callId = req->getCallId();
            std::string rawOffer = req->getOffer();

            //fprintf(stderr, "Media: RequestHandler: rawOffer='%s'\n", rawOffer.c_str());

            const char * data = rawOffer.c_str();
            size_t dataSize = rawOffer.size();
            Sdp sdpOffer(data, dataSize);
            Sdp sdpAnswer;

            uint32_t resourceId = mediaSessionManager->allocate(callId, sdpOffer, sdpAnswer);

            MediaAllocateResponse response(req->getDialogueId(), req->getCallbackId(), resourceId, sdpAnswer.toString(), 0, "OK");

            mediaServerInterface->PostResponse(stream, response);
        }
        break;

        case MediaMessage::Method::RELEASE: {

            auto * req = (MediaReleaseRequest *) request;
            //TODO locate resource and valid ownerReference

            mediaSessionManager->release(req->getResourceId());

            MediaReleaseResponse response(req->getDialogueId(), req->getCallbackId(), req->getResourceId(), 0, "OK");

            mediaServerInterface->PostResponse(stream, response);
        }
        break;

        case MediaMessage::Method::RECORD_START: {

            auto * req = (MediaRecordStartRequest *) request;

            MediaSession session = mediaSessionManager->retrieveSession(req->getResourceId());



            //TODO validate resource, kick off recording and get status
            MediaRecordStartResponse response(
                   req->getDialogueId(),
                   req->getCallbackId(),
                   req->getResourceId(),
                   req->getFilename().c_str(),
                   "temporary-system-reference",
                   0, "OK");

            mediaServerInterface->PostResponse(stream, response);
        }
        break;

        case MediaMessage::Method::RECORD_STOP: {

            MediaRecordStopRequest * req = (MediaRecordStopRequest *) request;

            //TODO validate resource, stop recording and get status
            MediaRecordStopResponse response(
                    req->getDialogueId(),
                    req->getCallbackId(),
                    req->getResourceId(),
                    "TBC-GetTheFileName-MediaEngine",
                    "temporary-system-reference",
                    0, "OK");
            mediaServerInterface->PostResponse(stream, response);
        }
        break;

        case MediaMessage::Method::PLAY_START: {

        }
        break;

        case MediaMessage::Method::PLAY_STOP: {

        }
        break;

        case MediaMessage::Method::EVENTS: {

        }
        break;
    }
}

int main(int argc, char * argv[]) {

    int port = 10020;
    mediaServerInterface = new MediaServerInterface(port, mediaRequestHandler);
    mediaSessionManager = new MediaSessionManager();
    mediaSessionManager->initialise(100);

    uv_thread_t loop_thread;
    //uv_idle_t idler;
    //uv_tcp_t mediaControlt;

    struct sockaddr_in socket_address;

//    uv_timer_t * ms20timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
//    ms20timer->data = (void *) 1;
//    uv_timer_init(uv_default_loop(), ms20timer);
//    uv_timer_start(ms20timer, &on_ms20_timer, 20, 20);

    mediaServerInterface->Start();

    uv_thread_create(&loop_thread, loop_process, 0);

//    uv_tcp_init(uv_default_loop(), &server);
//    uv_ip4_addr("0.0.0.0", port, &serverAddress);
//    uv_tcp_bind(&mediaClient, (struct sockaddr *)&serverAddress, 0);
//    int error = uv_listen((uv_stream_t *)&mediaClient, 10, on_connect);

    fprintf(stderr, "Started\n");

    uv_thread_join(&loop_thread);

    fprintf(stderr, "Loop Thread Completed\n");

    delete mediaSessionManager;
    delete mediaServerInterface;

    return 0;
}