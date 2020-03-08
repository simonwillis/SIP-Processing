#include <iostream>

#include <uv.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <json/json.h>

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
#include "../components/control/notifications/CallStatisticsNotification.h"

using namespace std;

ControlInterface * controlInterface;
SipInterface * sipInterface;
MediaClientInterface * mediaInterface;

//TODO Set up resource repo
uint32_t theresourceId = 0;

uv_stream_t * controlStream = NULL;

void mediaResponseHandler(MediaResponse * mediaResponse) {
    auto logger = spdlog::get("stdlogger");
    logger->debug("Core::mediaResponseHandler triggered");

    if (! mediaResponse) {
        logger->error("Core::mediaResponseHandler received invalid NULL response. Ignoring");
        return;
    }

    logger->debug("Core::mediaResponseHandler received  {} {} {}", mediaResponse->getDomainStr(), mediaResponse->getMethodStr(), mediaResponse->getTypeStr());

    switch (mediaResponse->getMethod()) {
        case MediaMessage::Method::UNKNOWN:
            break;

        case MediaMessage::Method::ALLOCATE: {

            logger->debug("Core::mediaResponseHandler ALLOCATE Response");

            MediaAllocateResponse * r = (MediaAllocateResponse *) mediaResponse;
            uint32_t channelId = r->getCallbackId();
            uint32_t resourceId = r->getResourceId();
            int dialogueId = r->getDialogueId();

            //TODO Update resource repo
            theresourceId = resourceId;

            logger->debug("Media resource allocated channelId={}, resourceId={}", channelId, resourceId);

            sipInterface->answerCall(channelId, "TBC-TEST-TAG", "TBC-TEST-CONTACT", r->getAnswer());

        }
        break;

        case MediaMessage::Method::RELEASE:

            logger->debug("Core::mediaResponseHandler RELEASE Response");
            break;

        case MediaMessage::Method::EVENTS:
            break;

        case MediaMessage::Method::PLAY_START:
            break;

        case MediaMessage::Method::PLAY_STOP:
            break;

        case MediaMessage::Method::RECORD_START: {
            logger->debug("Core::mediaResponseHandler RECORD_START Response - triggering test of RECORD_STOP");

            auto * response = (MediaRecordStartResponse *) mediaResponse;

            logger->debug("Core: Record Start Response callbackId={}, error={}, dialogueId={}, channelId={}, resourceId={}, filename={}",
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
            logger->debug("Core::mediaResponseHandler RECORD_STOP Response - triggering test of RELEASE");

            auto * r = (MediaRecordStopResponse *) mediaResponse;

            auto notification = CallRecordingCompletedNotification(r->getCallbackId(), r->getErrorCode(), r->getErrorMessage().c_str(), r->getDialogueId() );

            controlInterface->PostNotification(controlStream, notification);
        }
        break;

        case MediaMessage::Method::REGISTER: {

            logger->debug("Core::mediaResponseHandler REGISTER Response - received"); // triggering test of ALLOCATE\n");


        }
        break;

    }

}

void sipEventHandler(CallEvent * callEvent) {
    auto logger = spdlog::get("stdlogger");
    logger->debug("SIP Event Handler triggered with {}", callEvent->getEventTypeName());

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
                logger->debug("SIP Event Handler Incoming call [SDP={}]\n", e->getBody(SipMessageBody::BodyType::SDP).c_str());
            } else {
                logger->warn("SIP Event Handler Incoming call does not have any SDP");
            }
            if (e->hasBody(SipMessageBody::BodyType::XML)) {
                logger->debug("SIP Event Handler Incoming call XML={}", e->getBody(SipMessageBody::BodyType::XML).c_str());
            } else {
                logger->debug("SIP Event Handler Incoming call does not have any XML");
            }
            controlInterface->PostNotification(controlStream, notification);
        }
        break;

        case CallEvent::CallEventType::CALLANSWERED: {
            logger->debug("Call Answered");
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
            logger->debug("Call Cleared");
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
            logger->debug("Call Media Stats");
            CallMediaStatsEvent *e = (CallMediaStatsEvent *) callEvent;

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
            logger->debug("Media Recording Started");
            CallMediaRecordingStartedEvent * e = (CallMediaRecordingStartedEvent *) callEvent;
            CallRecordingStartedNotification notification = CallRecordingStartedNotification(
                    e->getChannelId(),
                    e->getErrorCode(),
                    e->getErrorMessage().c_str(),
                    e->getDialogueId());
            controlInterface->PostNotification(controlStream, notification);
        }

        case CallEvent::CallEventType::MEDIA_RECORDING_STOPPED: {
            logger->debug("Media Recording Stopped");
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
            logger->debug("Media Play Started");
            break;
        case CallEvent::CallEventType ::MEDIA_PLAY_STOPPED:
            logger->debug("Media Play Stopped");
            break;
        case CallEvent::CallEventType::MEDIA_DTMF:
            logger->debug("DTMF Event");
            break;
        case CallEvent::CallEventType::UNKNOWN:
            logger->warn("Unknown Media Event received");
            break;
    }
}

void controlRequestHandler(Request * request, uv_stream_t * stream) {
    auto logger = spdlog::get("stdlogger");

    //TODO remove stream from handler - need to either have single control connection, or allow broadcast to all controllers
    controlStream = stream;

    if (! request) {
        logger->warn("Core::controlRequestHandler: Invalid empty request received, unable to process");
    }

    logger->debug("Control request Handler triggered with {} request", request->getMethodName());

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
            logger->warn( "SYSTEM_LOGINCHALLENGERESPONSE ignored for now");
            break;


        case Request::CALL_PROGRESS:
            {
                //TODO Add handling for CALL_PROGRESS Request
                logger->warn("CALL_PROGRESS Request ignored for now");
            }
            break;

        case Request::CALL_RING:
            {
                //TODO Add handling for CALL_RING Request
                logger->warn("CALL_RING Request ignored for now");
            }
            break;

        case Request::CALL_ANSWER:
            {
                CallAnswerRequest * r = (CallAnswerRequest *) request;
                //TODO Add correct handling for CALL_ANSWER Request

                logger->debug("ControlRequest::CALL_ANSWER Request, channelId={}", r->getChannelId());

                std::string callId = r->getCallId();
                std::string sdpOffer = sipInterface->getMediaOffer(r->getChannelId());
                MediaAllocateRequest mediaAllocateRequest = MediaAllocateRequest(r->getDialogueId(), r->getChannelId(), callId, sdpOffer);
                mediaInterface->PostRequest(mediaAllocateRequest);
            }
            break;

        case Request::CALL_CLEAR:
            {
                //TODO Add handling for CALL_CLEAR Request
                logger->warn("CALL_CLEAR ignored for now");
            }
            break;

        case Request::CALL_RECORD:
            {
                CallRecordRequest * crr = (CallRecordRequest *) request;

                logger->debug("ControlRequest::CALL_RECORD");

                if (crr->isStart()) {
                    logger->debug("ControlRequest::CALL_RECORD - Start Recording");
                    auto r = MediaRecordStartRequest(crr->getDialogueId(), crr->getChannelId(), theresourceId, crr->getFilename().c_str());
                    mediaInterface->PostRequest(r);
//                    sipInterface->startRecordingCall(controlRequest->getChannelId(), r->getFilename().c_str());
                } else {
                    logger->debug("ControlRequest::CALL_RECORD - Stop Recording");

                    auto r = MediaRecordStopRequest(crr->getDialogueId(), crr->getChannelId(), theresourceId, 0);
                    sipInterface->stopRecordingCall(crr->getChannelId());
                    mediaInterface->PostRequest(r);
                }
            }
            break;


        case Request::INFO_STATUS:
            //TODO Add handling for INFO_STATUS Request
            logger->debug("ControlInterface: Handling for method not implemented yet [method={}", request->getMethodName());
            break;

        case Request::UNKNOWN:
            //TODO Implement proper logging processes
            logger->warn("ControlInterface: Unknown request received, method={}", request->getMethodName());
            break;

    }
}

void background_process(void * data) {

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void idle_handler(uv_idle_t* handle) {

}

void on_timer(uv_timer_t * timer) {
    auto logger = spdlog::get("stdlogger");
    logger->info("Timer event");
    // hacking here, simulating incoming call (key 1) and clearing call (key 2)
//    long key = (long)timer->data;
//    logger->debug( "on_timer id={}", key);
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

    auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    auto stdlogger = std::make_shared<spdlog::logger>("stdlogger", sink);
    spdlog::register_logger(stdlogger);

    uv_thread_t loop_thread;
    uv_idle_t idler;
    int sipPort = SipInterface::DefaultPort;

    auto logger = spdlog::get("stdlogger");
    stdlogger->info("getlogger::helloworld");

    for (int arg = 1; arg < argc; ++arg) {
        std::string argValue = std::string(argv[arg]);
        if (argValue.compare("-p") == 0) {
            if (arg <= argc - 2) {
                sipPort = atoi(argv[++arg]);
            }
        }
    }
    stdlogger->info("Setting Local SIP Port to {}\n", sipPort);

    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, idle_handler);

    uv_timer_t * timerStartCall = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    timerStartCall->data = (void *) 1;
    uv_timer_init(uv_default_loop(), timerStartCall);
    uv_timer_start(timerStartCall, &on_timer, 3000, 0);

    mediaInterface = new MediaClientInterface("localhost", 10020, mediaResponseHandler);
    mediaInterface->Start();

    sipInterface = new SipInterface(sipPort, sipEventHandler);
    sipInterface->Start(sipPort);

    controlInterface = new ControlInterface(10010, controlRequestHandler);
    controlInterface->Start();

    uv_thread_create(&loop_thread, background_process, 0);

    logger->debug("Started\n");

    uv_thread_join(&loop_thread);

    mediaInterface->Stop();

    sipInterface->Stop();

    controlInterface->Stop();

    logger->info("Stopped");

    return 0;
}