//
// Created by Simon Willis on 20/05/2016.
//


#include "ControlInterface.h"
#include "notifications/Notification.hpp"
#include "notifications/LoginNotification.h"
#include "notifications/CallCountNotification.h"
#include "notifications/CallIncomingNotification.h"
#include "notifications/CallConnectedNotification.h"
#include "notifications/CallClearedNotification.h"
#include "notifications/CallRecordingStartedNotification.h"
#include "notifications/CallRecordingCompletedNotification.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

using namespace std;


#define MAX_CONTROLLER_MESSAGE_SIZE 8192

#define DEBUG_CONTROL true
#define DEBUG_CONTROL_API true

MessageHandler ControlInterface::handler;
uv_tcp_t ControlInterface::server;
struct sockaddr_in ControlInterface::serverAddress;
stream_collection_t ControlInterface::connectedStreams;
uv_stream_t * ControlInterface::connectedStream;

void ControlInterface::alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * buffer) {
    buffer->base = (char *)malloc(MAX_CONTROLLER_MESSAGE_SIZE);
    buffer->len = MAX_CONTROLLER_MESSAGE_SIZE;
}

void ControlInterface::on_read_close(uv_handle_t * handle) {

    uv_stream_t * stream = (uv_stream_t *) handle;

    //TODO remove stream on ControlInterface::on_read_close
    //removeStreamFromCollection(stream);
}

void ControlInterface::removeStreamFromCollection(uv_stream_t * stream) {

    //TODO Fix/Complete ControlInterface::removeStreamFromCollection
//    auto logger = spdlog::get("stdlogger");
//    if (DEBUG_CONTROL) logger->debug("ControlInterface::removeStreamFromCollection Removing broken stream from connectedStreams collection");
//    uv_tcp_t * client = connectedStreams[stream];
//    if (client) {
//        logger->debug("ControlInterface::removeStreamFromCollection deleting client object);
//        delete client;
//    }
//    // we didn't create the stream, it was given to us, do not delete
//    connectedStreams.erase(stream);
//    logger->debug("ControlInterface::removeStreamFromCollection completed");
}

void ControlInterface::on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * buffer) {

    auto logger = spdlog::get("stdlogger");
    if (bytes_read < 0) {
        logger->error("ControlInterface Stream Handler. Read error [errorCode=%{}] [errorMessage={}]", bytes_read, uv_err_name(bytes_read));
        uv_read_stop(stream);
        uv_close((uv_handle_t *)stream, on_read_close);
        return;
    }

    string message = string(buffer->base, bytes_read);

    free((void *)buffer->base);

    logger->debug("CTL RX <<<===\n{}", message);

    Request * request = Request::build_request(message.c_str());

    logger->debug("CTL Message turned into {} request for callId {}", request->getMethodName(), request->getCallId());

    handler(request, stream);

}

void ControlInterface::on_write(uv_write_t * req, int status) {

    auto logger = spdlog::get("stdlogger");
    uv_buf_t * uvBuf = (uv_buf_t *)req->data;

    if (status) {
        logger->error("Write completed with status {}", status);
        return;
    }
    if (req->data == NULL) {
        logger->debug("on_write was passed empty data on, ignoring");
        return;
    }

    if (uvBuf) {
        if (uvBuf->base == NULL) {
            logger->debug("on_write uvBuf base is null, ignoring");
        } else {
            free(uvBuf->base);
            uvBuf->base = NULL;
            uvBuf->len = 0;
        }
        free(uvBuf);
        req->data = NULL;
    }
    free(req);

}

void ControlInterface::on_connect(uv_stream_t * stream, int status) {

    auto logger = spdlog::get("stdlogger");
    logger->debug("ControlInterface::on_connect");

    uv_tcp_t * client = new uv_tcp_t;

    int error = uv_tcp_init(uv_default_loop(), client);

    if (error) {
        logger->error("on_controller_connect uv_tcp_init failed with error {}, {}", error, uv_strerror(error));
        delete client;
        return;
    }

    error = uv_accept(stream, (uv_stream_t *) client);

    if (error) {
        logger->error("on_controller_connect uv_accept failed with error {}, {}", error, uv_strerror(error));
        delete client;
        return;
    }

    error = uv_read_start((uv_stream_t *) client, alloc_buffer, on_read);

    if (error) {
        logger->error("on_controller_connect uv_read_start failed with error {}, {}", error, uv_strerror(error));
        delete client;
        return;
    }

    //TODO Fix ControlInterface on_connect to set the connected_stream value
    logger->error("ControlInterface::on_connect NOT setting connectedStream value - Needs to be completed");
    // connectedStreams[stream] = client;

}

ControlInterface::ControlInterface(int port, MessageHandler requestHandler)
        : port(port) {

    handler = requestHandler;
    logger = spdlog::get("stdlogger");
}

int ControlInterface::Start() {

    uv_tcp_init(uv_default_loop(), &server);

    uv_ip4_addr("0.0.0.0", port, &serverAddress);

    uv_tcp_bind(&server, (struct sockaddr *)&serverAddress, 0);

    int error = uv_listen((uv_stream_t *)&server, 10, on_connect);

    return (error == 0);
}

int ControlInterface::Stop() {

    return 0;
}

void ControlInterface::PostNotification(uv_stream_t * stream, Notification & notification) {
    if (stream == nullptr) {
        logger->error("ControlInterface::PostNotification Stream is null");
        return;
    }
    Send(stream, notification.toJson());
}

int ControlInterface::Send(uv_stream_t * stream, string message) {

    if (! stream) {
        logger->error("ControlInterface::Send - control stream does not exist");
        return - 1;
    }

    logger->debug("ControlInterface::Send:  message length={}", message.length());

    uv_buf_t * uvBuf = (uv_buf_t *)malloc(sizeof(uv_buf_t));
    uvBuf->base = (char *) malloc(message.length());
    memcpy((void *) uvBuf->base, message.c_str(), message.length());
    uvBuf->len = message.length();

    logger->debug("CTL TX ===>>>\n{}", string(uvBuf->base, uvBuf->len));

    uv_write_t * req = (uv_write_t *) malloc(sizeof(uv_write_t));

    req->data = (void *) uvBuf;
    int error = uv_write(req, stream, uvBuf, 1, on_write);
    if (error) {
        logger->error("ControlInterface::Send uv_write returned {}, {}", error, uv_err_name(error));
    }

    return 0;
}