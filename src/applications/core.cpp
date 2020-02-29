#include <iostream>

#include <uv.h>

#include <spdlog/spdlog.h>
#include <json/json.h>

#include "../../tools/RaiiLog.h"

#include "../components/control/requests/Request.h"
#include "../components/control/ControlInterface.h"
#include "../components/sip/events/CallEvent.h"
#include "../components/sip/SipInterface.h"
#include "../components/control/notifications/CallIncomingNotification.h"
#include "../components/sip/events/CallNewCallEvent.h"
#include "../components/control/requests/CallAnswerRequest.h"
#include "../components/control/requests/CallRecordRequest.h"
#include "../components/sip/events/CallAnsweredEvent.h"
#include "../components/sip/events/CallClearedEvent.h"
#include "../components/sip/events/CallMediaRecordingStartedEvent.h"
#include "../components/sip/events/CallMediaStatsEvent.h"
#include "../components/control/notifications/CallClearedNotification.h"
#include "../components/control/notifications/CallConnectedNotification.h"
#include "../components/control/notifications/CallRecordingStartedNotification.h"
#include "../components/control/notifications/CallRecordingCompletedNotification.h"
#include "../components/control/notifications/CallCountNotification.h"
#include "../components/control/notifications/LoginNotification.h"
#include "../components/sip/events/CallMediaRecordingStoppedEvent.h"
#include "../components/media/MediaClientInterface.h"
#include "../components/media/messages/MediaAllocateRequest.h"
#include "../components/media/messages/MediaAllocateResponse.h"
#include "../components/media/messages/MediaReleaseRequest.h"
#include "../components/media/messages/MediaRecordStopResponse.h"
#include "../components/media/messages/MediaRecordStartRequest.h"
#include "../components/media/messages/MediaRecordStartResponse.h"
#include "../components/media/messages/MediaRecordStopRequest.h"
#include "../components/media/definitions.h"
#include "../components/control/notifications/CallStatisticsNotification.h"

using namespace std;

/*
 * For information on using spdlog see https://github.com/gabime/spdlog/wiki/1.-QuickStart
 */

ControlInterface * controlInterface;
SipInterface * sipInterface;
MediaClientInterface * mediaInterface;

//TODO Set up resource repo
uint32_t theresourceId = 0;

uv_stream_t * controlStream = NULL;

void mediaResponseHandler(MediaResponse * mediaResponse) {
     RaiiLog raiiLogr = RaiiLog("Core::mediaResponseHandler");
    //fprintf(stderr, "Core::mediaResponseHandler triggered\n");

    if (! mediaResponse) {
        fprintf(stderr, "Core::mediaResponseHandler received invalid NULL response. Ignoring\n");
        return;
    }

    //fprintf(stderr, "Core::mediaResponseHandler received  %s %s %s\n", mediaResponse->getDomainStr(), mediaResponse->getMethodStr(), mediaResponse->getTypeStr());

    switch (mediaResponse->getMethod()) {
        case MediaMessage::Method::UNKNOWN:
            break;

        case MediaMessage::Method::ALLOCATE: {

            fprintf(stderr, "Core::mediaResponseHandler ALLOCATE Response\n");

            MediaAllocateResponse * r = (MediaAllocateResponse *) mediaResponse;
            uint32_t channelId = r->getCallbackId();
            uint32_t resourceId = r->getResourceId();
            int dialogueId = r->getDialogueId();

            //TODO Update resource repo
            theresourceId = resourceId;

            fprintf(stderr, "Media resource allocated channelId=%zu, resourceId=%zu\n", channelId, resourceId);

            sipInterface->answerCall(channelId, "TBC-TEST-TAG", "TBC-TEST-CONTACT", r->getAnswer());

        }
        break;

        case MediaMessage::Method::RELEASE:

            fprintf(stderr, "Core::mediaResponseHandler RELEASE Response\n");
            break;

        case MediaMessage::Method::EVENTS:
            break;

        case MediaMessage::Method::PLAY_START:
            break;

        case MediaMessage::Method::PLAY_STOP:
            break;

        case MediaMessage::Method::RECORD_START: {
            fprintf(stderr, "Core::mediaResponseHandler RECORD_START Response - triggering test of RECORD_STOP\n");

            auto * response = (MediaRecordStartResponse *) mediaResponse;

            fprintf(stderr, "Core: Record Start Response callbackId=%d, error=%zu, dialogueId=%d, channelId=%u, resourceId=%u, filename=%s\n",
                response->getCallbackId(),
                response->getErrorCode(),
                response->getDialogueId(),
                response->getCallbackId(),
                response->getResourceId(),
                response->getFilename().c_str());

            auto notification = CallRecordingStartedNotification(response->getCallbackId(), response->getErrorCode());

            controlInterface->PostNotification(controlStream, notification);

        }
        break;

        case MediaMessage::Method::RECORD_STOP: {
            fprintf(stderr, "Core::mediaResponseHandler RECORD_STOP Response - triggering test of RELEASE\n");

            auto * r = (MediaRecordStopResponse *) mediaResponse;

            auto notification = CallRecordingCompletedNotification(r->getCallbackId(), r->getErrorCode(), r->getErrorMessage().c_str(), r->getDialogueId() );

            controlInterface->PostNotification(controlStream, notification);
        }
        break;

        case MediaMessage::Method::REGISTER: {

            fprintf(stderr, "Core::mediaResponseHandler REGISTER Response - received\n"); // triggering test of ALLOCATE\n");


        }
        break;

    }

}

void sipEventHandler(CallEvent * callEvent) {
    RaiiLog raiiLog = RaiiLog("Core::sipEventHandler");
    fprintf(stderr, "SIP Event Handler triggered with %s\n", callEvent->getEventTypeName());
    switch(callEvent->getEventType()) {

        case CallEvent::CallEventType::NEWCALL: {
            CallNewCallEvent * e = (CallNewCallEvent *)callEvent;

            CallIncomingNotification notification = CallIncomingNotification(
                    e->getChannelId(),
                    e->getCallId().c_str(),
                    e->getTag().c_str(),
                    e->getTo().c_str(),
                    e->getFrom().c_str(),
                    e->getHeaders(),
                    e->getDialogueId());

            //e->getMediaOffer();

            //TODO pull back SDP Offer and any extra data, raw copy of extra bodies needed. (base 64 encoding ?)

            if (e->hasBody(SipMessageBody::BodyType::SDP)) {
                fprintf(stderr, "SIP Event Handler Incoming call [SDP='%s']\n", e->getBody(SipMessageBody::BodyType::SDP).c_str());
            } else {
                fprintf(stderr, "SIP Event Handler Incoming call does not have any SDP\n");
            }
            if (e->hasBody(SipMessageBody::BodyType::XML)) {
                fprintf(stderr, "SIP Event Handler Incoming call [XML='%s']\n", e->getBody(SipMessageBody::BodyType::XML).c_str());
            } else {
                fprintf(stderr, "SIP Event Handler Incoming call does not have any XML\n");
            }
            controlInterface->PostNotification(controlStream, notification);
        }
        break;

        case CallEvent::CallEventType::CALLANSWERED: {
            fprintf(stderr, "Call Answered\n");
            CallAnsweredEvent * e = (CallAnsweredEvent *) callEvent;
            CallConnectedNotification notification = CallConnectedNotification(
                    e->getChannelId(),
                    e->getErrorCode(),
                    e->getErrorMessage().c_str(),
                    e->getDialogueId());

            controlInterface->PostNotification(controlStream, notification);
        }
        break;

        case CallEvent::CallEventType::CALLCLEARED: {
            fprintf(stderr, "Call Cleared\n");
            CallClearedEvent *e = (CallClearedEvent *) callEvent;
            CallClearedNotification notification = CallClearedNotification(
                    e->getChannelId(),
                    e->getClearingCode(),
                    e->getErrorCode(),
                    e->getErrorMessage().c_str(),
                    e->getDialogueId());
            controlInterface->PostNotification(controlStream, notification);
        }
        break;

        case CallEvent::CallEventType::MEDIA_ERROR:
            break;
//        case CallEvent::CallEventType::MEDIA_OFFER:
//            break;
        case CallEvent::CallEventType::MEDIA_STATS: {
            CallMediaStatsEvent *e = (CallMediaStatsEvent *) callEvent;

            fprintf(stderr, "Core: SipEventHandler: CallEvent::CallEventType::MEDIA_STATS: NOT HANDLED\n");
            //TODO Add Call Stats Notification
            CallStatisticsNotification notification = CallStatisticsNotification(
                    e->getChannelId(),
                    e->getStats(),
                    e->getErrorCode(),
                    e->getErrorMessage().c_str(),
                    e->getDialogueId()
            );
        }
        break;

        case CallEvent::CallEventType::MEDIA_RECORDING_STARTED: {
            fprintf(stderr, "Call Recording Started\n");
            CallMediaRecordingStartedEvent * e = (CallMediaRecordingStartedEvent *) callEvent;
            CallRecordingStartedNotification notification = CallRecordingStartedNotification(
                    e->getChannelId(),
                    e->getErrorCode(),
                    e->getErrorMessage().c_str(),
                    e->getDialogueId());
            controlInterface->PostNotification(controlStream, notification);
        }

        case CallEvent::CallEventType::MEDIA_RECORDING_STOPPED: {
            fprintf(stderr, "Call Recording Stopped\n");
            CallMediaRecordingStoppedEvent * e = (CallMediaRecordingStoppedEvent *) callEvent;
            CallRecordingCompletedNotification notification = CallRecordingCompletedNotification(
                    e->getChannelId(),
                    e->getErrorCode(),
                    e->getErrorMessage().c_str(),
                    e->getDialogueId());
            controlInterface->PostNotification(controlStream, notification);
        }
        break;
        case CallEvent::CallEventType::MEDIA_PLAY_STARTED:
            break;
        case CallEvent::CallEventType ::MEDIA_PLAY_STOPPED:
            break;
        case CallEvent::CallEventType::MEDIA_DTMF:
            break;
        case CallEvent::CallEventType::UNKNOWN:
            break;
    }
}

void controlRequestHandler(Request * request, uv_stream_t * stream) {
    auto raiiLog = RaiiLog("Core::controlRequestHandler");
    //TODO remove stream from handler - need to either have single control connection, or allow broadcast to all controllers
    controlStream = stream;

    if (! request) {
        fprintf(stderr, "Core::controlRequestHandler: Invalid empty request received, unable to process\n");
    }

    fprintf(stderr, "\nControl request Handler triggered with %s request\n", request->getMethodName());

    switch (request->getMethod()) {

        case Request::SYSTEM_LOGIN:
            {
                LoginNotification notification = LoginNotification();
                controlInterface->PostNotification(controlStream, notification);
            }
            break;

        case Request::INFO_CALLCOUNT:
            {
                //TODO - pick up the call count when sending call count information
                CallCountNotification notification = CallCountNotification(0);
                controlInterface->PostNotification(controlStream, notification);
            }
            break;

        case Request::SYSTEM_LOGINCHALLENGERESPONSE:
            //TODO Implement login challenge process
            fprintf(stderr, "SYSTEM_LOGINCHALLENGERESPONSE ignored for now\n");
            break;


        case Request::CALL_PROGRESS:
            {
                //TODO Add handling for CALL_PROGRESS Request
                fprintf(stderr, "CALL_PROGRESS Request ignored for now\n");
            }
            break;

        case Request::CALL_RING:
            {
                //TODO Add handling for CALL_RING Request
                fprintf(stderr, "CALL_RING Request ignored for now\n");
            }
            break;

        case Request::CALL_ANSWER:
            {
                CallAnswerRequest * r = (CallAnswerRequest *) request;
                //TODO Add correct handling for CALL_ANSWER Request

                fprintf(stderr, "ControlRequest::CALL_ANSWER Request, channelId=%d, r->channelId=%d\n", request->getChannelId(), r->getChannelId());

                std::string callId = r->getCallId();
                std::string sdpOffer = sipInterface->getMediaOffer(r->getChannelId());
                MediaAllocateRequest mediaAllocateRequest = MediaAllocateRequest(r->getDialogueId(), r->getChannelId(), callId, sdpOffer);
                mediaInterface->PostRequest(mediaAllocateRequest);
            }
            break;

        case Request::CALL_CLEAR:
            {
                //TODO Add handling for CALL_CLEAR Request
                fprintf(stderr, "CALL_CLEAR ignored for now\n");
            }
            break;

        case Request::CALL_RECORD:
            {
                CallRecordRequest * crr = (CallRecordRequest *) request;

                fprintf(stderr, "ControlRequest::CALL_RECORD\n");

                if (crr->isStart()) {
                    fprintf(stderr, "ControlRequest::CALL_RECORD - Start Recording\n");
                    auto r = MediaRecordStartRequest(crr->getDialogueId(), crr->getChannelId(), theresourceId, crr->getFilename().c_str());
                    mediaInterface->PostRequest(r);
//                    sipInterface->startRecordingCall(controlRequest->getChannelId(), r->getFilename().c_str());
                } else {
                    fprintf(stderr, "ControlRequest::CALL_RECORD - Stop Recording\n");

                    auto r = MediaRecordStopRequest(crr->getDialogueId(), crr->getChannelId(), theresourceId, 0);
                    sipInterface->stopRecordingCall(crr->getChannelId());
                    mediaInterface->PostRequest(r);
                }
            }
            break;


        case Request::INFO_STATUS:
            //TODO Add handling for INFO_STATUS Request
            fprintf(stderr, "ControlInterface: work to do\n");
            fprintf(stderr, "ControlInterface: Handling for method not implemented yet [method=%s\n", request->getMethodName());
            break;

        case Request::UNKNOWN:
            //TODO Implement proper logging processes
            fprintf(stderr, "ControlInterface: Unknown request received [method=%s\n", request->getMethodName());
            break;

    }
}

void background_process(void * data) {
    //fprintf(stderr, "background_process Started\n");

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    //fprintf(stderr, "background_process Stopped\n");
}

void idle_handler(uv_idle_t* handle) {
    //fprintf(stderr, "Idle Handler event\n");
}

void on_timer(uv_timer_t * timer) {
    // hacking here, simulating incoming call (key 1) and clearing call (key 2)
//    long key = (long)timer->data;
//    fprintf(stderr, "on_timer id=%ld\n", key);
//    if (key == 1) {
//        controlInterface->PostCallIncomingNotification(controlStream, 1, "test-call-id");
//    } else if (key == 2) {
//        controlInterface->PostCallRecordingCompleted(controlStream, 1, "test-call-id");
//        controlInterface->PostCallClearedNotification(controlStream, 1, "test-call-id", 200, 0);
//    }
}

void on_timer_close(uv_handle_t * handle) {
    free(handle);
}

int main(int argc, char * argv[]) {

    uv_thread_t loop_thread;
    uv_idle_t idler;
    int sipPort = SipInterface::DefaultPort;

    for (int arg = 1; arg < argc; ++arg) {
        std::string argValue = std::string(argv[arg]);
        if (argValue.compare("-p") == 0) {
            if (arg <= argc - 2) {
                sipPort = atoi(argv[++arg]);
            }
        }
    }
    fprintf(stderr, "Setting Local SIP Port to %d\n", sipPort);


//    uv_idle_init(uv_default_loop(), &idler);
//    uv_idle_start(&idler, idle_handler);

//    uv_timer_t * timerStartCall = (uv_timer_t *)malloc(sizeof(uv_timer_t));
//    timerStartCall->data = (void *) 1;
//    uv_timer_init(uv_default_loop(), timerStartCall);
//    uv_timer_start(timerStartCall, &on_timer, 30000, 0);
//
//
//    uv_timer_t * timerEndCall = (uv_timer_t *)malloc(sizeof(uv_timer_t));
//    timerEndCall->data = (void *) 2;
//    uv_timer_init(uv_default_loop(), timerEndCall);
//    uv_timer_start(timerEndCall, &on_timer, 40000, 0);



    mediaInterface = new MediaClientInterface("localhost", 10020, mediaResponseHandler);
    mediaInterface->Start();

    sipInterface = new SipInterface(sipPort, sipEventHandler);
    sipInterface->Start(sipPort);

    controlInterface = new ControlInterface(10010, controlRequestHandler);
    controlInterface->Start();

    uv_thread_create(&loop_thread, background_process, 0);

    fprintf(stderr, "Started\n");

    uv_thread_join(&loop_thread);

    mediaInterface->Stop();

    sipInterface->Stop();

    controlInterface->Stop();

    fprintf(stderr, "Stopped\n");
    return 0;
}