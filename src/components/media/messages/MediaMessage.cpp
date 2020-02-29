//
// Created by Simon Willis on 09/06/2016.
//

#include "MediaMessage.h"
#include "MediaRegisterRequest.h"
#include "MediaRegisterResponse.h"
#include "MediaAllocateRequest.h"
#include "MediaAllocateResponse.h"
#include "MediaRecordStartResponse.h"
#include "MediaRecordStopResponse.h"
#include "MediaRecordStartRequest.h"
#include "MediaRecordStopRequest.h"
#include "MediaReleaseRequest.h"
#include "MediaReleaseResponse.h"

using namespace std;


MediaMessage::MediaMessage(std::string json) : jsonData(json) {
    //fprintf(stderr, "MediaMessage::CTOR [json=%s]\n", json.c_str());
}

string MediaMessage::toJson() {
    Json::Value root;
    root["type"] = getTypeStr();
    root["domain"] = getDomainStr();
    root["method"] = getMethodStr();
    root["dialogueId"] = getDialogueId();
    root["callbackId"] = getCallbackId();

    if (hasData()) root["data"] = getData();
    if (hasStatus()) root["status"] = getStatus();

    Json::FastWriter writer;

    return writer.write(root);
}

const char * MediaMessage::getTypeText(Type type) {
    switch (type) {
        case MediaMessage::Type::UNKNOWN:
            return "unknown";
        case MediaMessage::Type::REQUEST:
            return "request";

        case MediaMessage::Type::RESPONSE:
        default:
            return "response";
    }
}

const char * MediaMessage::getMethodText(Method method) {
    switch (method) {
        case MediaMessage::Method::UNKNOWN:
            return "unknown";
        case MediaMessage::Method::REGISTER:
            return "register";
        case MediaMessage::Method::ALLOCATE:
            return "allocate";
        case MediaMessage::Method::RELEASE:
            return "release";
        case MediaMessage::Method::RECORD_START:
            return "recordStart";
        case MediaMessage::Method::RECORD_STOP:
            return "recordStop";
        case MediaMessage::Method::PLAY_START:
            return "playStart";
        case MediaMessage::Method::PLAY_STOP:
            return "playStop";

        case MediaMessage::Method::EVENTS:
        default:
            return "events";
    }
}

MediaMessage * MediaMessage::build_message(const char * json) {
    Json::Value root;
    Json::Value data = Json::Value::nullRef;
    Json::Reader reader;
    MediaResponse * message = NULL;

    bool parsed_ok = reader.parse(json, root);
    if (! parsed_ok) {
        fprintf(stderr, "Failed to parse message into object\n");
        return message;
    }

    //fprintf(stderr, "MediaMessage: building message from %s\n", json);

    string domain = string(root.get("domain", "UNKNOWN").asCString());
    string type = string(root.get("type", "UNKNOWN").asCString());
    string method = string(root.get("method", "UNKNOWN").asCString());
    int dialogueId = root.get("dialogueId", 0).asInt();
    uint32_t callbackId = root.get("callbackId", 0).asUInt();

    //fprintf(stderr, "MediaMessage: domain=%s, type=%s, method=%s, dialogueId=%d, callbackId=%u\n", domain.c_str(), type.c_str(), method.c_str(), dialogueId, callbackId);

    if (root.isMember("data")) {
        data = root.get("data", Json::Value::nullRef);
    }

    if (type.compare("response") == 0) {

        int errorCode = 0;
        string errorMessage;
        if (root.isMember("status")) {
            Json::Value status = root.get("status", Json::Value::nullRef);
            errorCode = status.get("errorCode", 0).asInt();
            errorMessage = string(status.get("errorMessage", "").asCString());
        }

        if (method.compare("register") == 0) return new MediaRegisterResponse(dialogueId, callbackId, errorCode, errorMessage.c_str());
        if (method.compare("allocate") == 0) {
            std::string sdp = data.get("offerAnswer", "EMPTY SDP").asString();
            uint32_t resourceId = data.get("resourceId", 0).asUInt();
            return new MediaAllocateResponse(dialogueId, callbackId, resourceId, sdp, errorCode, errorMessage.c_str());
        }
        if (method.compare("release") == 0) return new MediaReleaseResponse(data, dialogueId, callbackId, errorCode, errorMessage.c_str());
        if (method.compare("recordStart") == 0) return new MediaRecordStartResponse(data, dialogueId, callbackId, errorCode, errorMessage.c_str());
        if (method.compare("recordStop") == 0) return new MediaRecordStopResponse(data, dialogueId, callbackId, errorCode, errorMessage.c_str());

        fprintf(stderr, "MediaMessage::buildMediaMessage Unable to handle request - [method=%s] [length=%ld]\n", method.c_str(), method.size());

    } else if (type.compare("request") == 0) {

        //TODO get a valid registration identifier
        if (method.compare("register") == 0) return new MediaRegisterRequest("temporaryIdentifier", dialogueId, callbackId);
        if (method.compare("allocate") == 0) {
            std::string callId = data.get("callId", std::string()).asString();
            std::string sdp = data.get("sdp", std::string()).asString();

            return new MediaAllocateRequest(dialogueId, callbackId, callId, sdp);
        }
        if (method.compare("release") == 0) return new MediaReleaseRequest(callbackId, dialogueId, data);
        if (method.compare("recordStart") == 0) return new MediaRecordStartRequest(callbackId, dialogueId, data);
        if (method.compare("recordStop") == 0) return new MediaRecordStopRequest(callbackId, dialogueId, data);

        fprintf(stderr, "MediaRequest::build_request Unable to handle request - [method=%s] [length=%ld]\n", method.c_str(), method.size());
    } else {

        fprintf(stderr, "MediaMessage::build_message unexpected message [Type=%s] in [JSON=%s]\n", type.c_str(), json);
    }



    return message;
}