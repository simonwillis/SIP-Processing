//
// Created by Simon Willis on 19/05/2016.
//

#include "Request.h"
#include "LoginRequest.h"
#include "CallCountRequest.h"
#include "CallProgressRequest.h"
#include "CallRingRequest.h"
#include "CallAnswerRequest.h"
#include "CallRecordRequest.h"
#include "CallClearRequest.h"

#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

using namespace std;


Request * Request::build_request(const char *json) {
    Json::Value root;
    Json::Value data = Json::Value::nullRef;
    Json::Reader reader;
    Request * req = NULL;

    bool parsed_ok = reader.parse(json, root);
    if (! parsed_ok) {
        spdlog::get("stdlogger")->warn("Failed to parse json into object {}", json);
        return req;
    }

    string messageType = string(root.get("type", "UNKNOWN").asCString());
    string method = string(root.get("method", "UNKNOWN").asCString());
    int dialogueId = root.get("dialogueId", 0).asInt();

    if (root.isMember("data")) {
        data = root.get("data", Json::Value::nullRef);
    }

    if (messageType.compare("request") == 0) {
        if (method.compare("callCount") == 0) return new CallCountRequest(dialogueId);
        if (method.compare("answerCall") == 0) return new CallAnswerRequest(data, dialogueId);
        if (method.compare("recordCall") == 0) return new CallRecordRequest(data, dialogueId);
        if (method.compare("clearCall") == 0) return new CallClearRequest(data, dialogueId);
        if (method.compare("login") == 0) return new LoginRequest(data, dialogueId);
        if (method.compare("sendRingback") == 0) return new CallRingRequest(data, dialogueId);
        if (method.compare("sendProgress") == 0) return new CallProgressRequest(data, dialogueId);

        spdlog::get("stdlogger")->warn("Request::build_request Unable to handle request - method {}, length {}", method.c_str(), method.size());
    }

    return req;
}