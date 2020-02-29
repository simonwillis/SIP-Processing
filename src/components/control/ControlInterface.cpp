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
#include "../../../tools/RaiiLog.h"

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
    fprintf(stderr, "ControlInterface::on_read_close\n");
    uv_stream_t * stream = (uv_stream_t *) handle;

    //removeStreamFromCollection(stream);
    //fprintf(stderr, "ControlInterface::on_read_close - complete\n");
}

void ControlInterface::removeStreamFromCollection(uv_stream_t * stream) {
//    if (DEBUG_CONTROL) fprintf(stderr, "ControlInterface::removeStreamFromCollection Removing broken stream from connectedStreams collection\n");
//    uv_tcp_t * client = connectedStreams[stream];
//    if (client) {
//        if (DEBUG_CONTROL) fprintf(stderr, "ControlInterface::removeStreamFromCollection deleting client object\n");
//        delete client;
//    }
//    // we didn't create the stream, it was given to us, do not delete
//    connectedStreams.erase(stream);
//    if (DEBUG_CONTROL) fprintf(stderr, "ControlInterface::removeStreamFromCollection completed\n");
}

void ControlInterface::on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * buffer) {

    if (bytes_read < 0) {
        fprintf(stderr, "ControlInterface Stream Handler. Read error [errorCode=%ld] [errorMessage=%s]\n", bytes_read, uv_err_name(bytes_read));
        uv_read_stop(stream);
        uv_close((uv_handle_t *)stream, on_read_close);
        return;
    }

    string message = string(buffer->base, bytes_read);

    free((void *)buffer->base);

    if (DEBUG_CONTROL_API) fprintf(stderr, "CTL RX <<<===\n%s\n", message.c_str());

    Request * request = Request::build_request(message.c_str());

    if (DEBUG_CONTROL) fprintf(stderr, "CTL Message turned into request\n");

    handler(request, stream);

}

void ControlInterface::on_write(uv_write_t * req, int status) {
    fprintf(stderr, "ControlInterface::on_write\n");
    uv_buf_t * uvBuf = (uv_buf_t *)req->data;
    if (status) {
        fprintf(stderr, "Write completed with status %d\n", status);
        return;
    }
    if (req->data == NULL) {
        fprintf(stderr, "on_write was passed empty data on, ignoring\n");
        return;
    }

    if (uvBuf) {
        if (uvBuf->base == NULL) {
            fprintf(stderr, "on_write uvBuf base is null, ignoring\n");
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

    fprintf(stderr, "ControlInterface::on_connect\n");

    uv_tcp_t * client = new uv_tcp_t;

    int error = uv_tcp_init(uv_default_loop(), client);

    if (error) {
        fprintf(stderr, "on_controller_connect uv_tcp_init failed with error %d %s\n", error, uv_strerror(error));
        delete client;
        return;
    }

    error = uv_accept(stream, (uv_stream_t *) client);

    if (error) {
        fprintf(stderr, "on_controller_connect uv_accept failed with error %d %s\n", error, uv_strerror(error));
        delete client;
        return;
    }

    error = uv_read_start((uv_stream_t *) client, alloc_buffer, on_read);

    if (error) {
        fprintf(stderr, "on_controller_connect uv_read_start failed with error %d %s\n", error, uv_strerror(error));
        delete client;
        return;
    }


    fprintf(stderr, "ControlInterface::on_connect setting connectedStream value %zu\n", stream);

   // connectedStreams[stream] = client;

}

ControlInterface::ControlInterface(int port, MessageHandler requestHandler)
        : port(port) {

    handler = requestHandler;
}

int ControlInterface::Start() {

    uv_tcp_init(uv_default_loop(), &server);

    uv_ip4_addr("0.0.0.0", port, &serverAddress);

    uv_tcp_bind(&server, (struct sockaddr *)&serverAddress, 0);

    int error = uv_listen((uv_stream_t *)&server, 10, on_connect);

    return (error == 0);
}

int ControlInterface::Stop() {
    //fprintf(stderr, "ControlInterface::Stop\n");

    return 0;
}

void ControlInterface::PostNotification(uv_stream_t * stream, Notification & notification) {
    fprintf(stderr, "ControlInterface::PostNotification Stream %s\n", stream ? "OK":"BAD");

    Send(stream, notification.toJson());
}

int ControlInterface::Send(uv_stream_t * stream, string message) {
    RaiiLog("ControlInterface::Send");

    if (! stream) {
        fprintf(stderr, "ControlInterface::Send - control stream does not exist\n");
        return - 1;
    }

    //fprintf(stderr, "ControlInterface::Send using stream value %zu\n", stream);

    //fprintf(stderr, "ControlInterface::Send:  message length=%d\n", message.length());
    uv_buf_t * uvBuf = (uv_buf_t *)malloc(sizeof(uv_buf_t));
    uvBuf->base = (char *) malloc(message.length());
    memcpy((void *) uvBuf->base, message.c_str(), message.length());
    uvBuf->len = message.length();

    if (DEBUG_CONTROL_API) fprintf(stderr, "CTL TX ===>>>\n%s\n", string(uvBuf->base, uvBuf->len).c_str());

    //fprintf(stderr, "ControlInterface::Send: allocating data %u bytes\n", sizeof(uv_write_t));
    uv_write_t * req = (uv_write_t *) malloc(sizeof(uv_write_t));

    //fprintf(stderr, "ControlInterface::Send: setting req->data\n");
    req->data = (void *) uvBuf;
    //fprintf(stderr, "ControlInterface::Send: calling uv_write\n");
    int error = uv_write(req, stream, uvBuf, 1, on_write);
    //fprintf(stderr, "ControlInterface::Send: call completed\n");
    if (error) {
        fprintf(stderr, "ControlInterface::Send uv_write returned %d (%s)\n", error, uv_err_name(error));
    }
//    else {
//        fprintf(stderr, "ControlInterface::Send returned without  error\n");
//    }

    return 0;

}