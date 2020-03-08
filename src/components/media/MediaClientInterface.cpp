//
// Created by Simon Willis on 09/06/2016.
//

#include "MediaClientInterface.h"
#include "messages/MediaAllocateResponse.h"
#include "messages/MediaRequest.h"
#include "messages/MediaRegisterRequest.h"

#include <stdio.h> // required for memcpy for some compilers
#include <cstring>

using namespace std;

MediaResponseHandler MediaClientInterface::handler;
uv_stream_t * MediaClientInterface::clientConnection;


MediaClientInterface::MediaClientInterface(std::string serverAddress, int serverPort, MediaResponseHandler responseHandler)
        : remoteAddress(serverAddress), remotePort(serverPort) {

    logger = spdlog::get("stdlogger");
    handler = responseHandler;
}

void MediaClientInterface::alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * buffer) {
    buffer->base = (char *)malloc(4096);
    buffer->len = 4096;
}

void MediaClientInterface::on_read_close(uv_handle_t * handle) {
    spdlog::get("stdlogger")->warn("MediaInterface::on_read_close Not Implemented Yet");
    uv_stream_t * stream = (uv_stream_t *) handle;
    //TODO Tidy up stream handle on close
}

void MediaClientInterface::on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * buffer) {

    if (bytes_read < 0) {
        spdlog::get("stdlogger")->warn("ControlInterface Stream Handler. Read error [errorCode={}] [errorMessage={}]", bytes_read, uv_err_name(bytes_read));
        uv_read_stop(stream);
        uv_close((uv_handle_t *)stream, on_read_close);
        return;
    }

    string json = string(buffer->base, bytes_read);

    free((void *)buffer->base);

    spdlog::get("stdlogger")->debug("MDC RX <<<=== {}", json);

    MediaMessage * msg = MediaMessage::build_message(json.c_str());

    if (msg) {
        if (msg->getType() == MediaMessage::Type::RESPONSE) {
            handler((MediaResponse *) msg);
        } else {
            spdlog::get("stdlogger")->warn("MediaClientInterface::onUdpRecv unexpected message [Type={}] expected [response]",
                    msg->getTypeStr());
        }
    } else {
        spdlog::get("stdlogger")->warn("MediaClientInterface::onUdpRecv failed to build message, result is NULL, json={}", json);
    }
}

void MediaClientInterface::on_write(uv_write_t * req, int status) {

    uv_buf_t * uvBuf = (uv_buf_t *)req->data;
    if (status) {
        spdlog::get("stdlogger")->warn("MediaInterface::on_write completed with status {}", status);
        return;
    }
    if (req->data == NULL) {
        spdlog::get("stdlogger")->debug("MediaInterface::on_write was passed empty data on, ignoring");
        return;
    }

    if (uvBuf) {
        if (uvBuf->base == NULL) {
            spdlog::get("stdlogger")->debug("MediaInterface::on_write uvBuf base is null, ignoring");
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

void MediaClientInterface::on_connect(uv_connect_t *connection, int status) {


    if (status) {
        spdlog::get("stdlogger")->warn("MediaInterface::on_connect Failed to Connect to Media Service errorMsg={}", uv_err_name(status));

    } else {

        MediaClientInterface *that = (MediaClientInterface *) connection->data;

        that->clientConnection = connection->handle;

        uv_timer_stop(that->connectTimer);

        uv_read_start(clientConnection, alloc_buffer, on_read);

        //TODO Grab the System Identifier on Startup and pass to register request
        //TODO Create a dialogueId for Media Requests
        string tbcIdentifier = string("systemIdentifier");
        int tbcDialogueId = 1;
        uint32_t tbcCallbackId = 0;
        MediaRegisterRequest request = MediaRegisterRequest(tbcIdentifier.c_str(), tbcCallbackId, tbcDialogueId);
        that->PostRequest(request);

    }
}

void MediaClientInterface::try_connect(uv_timer_t * timer) {

    int status;


    MediaClientInterface * that = (MediaClientInterface *)timer->data;

    status = uv_tcp_init(uv_default_loop(), &that->client);
    if (status) {
        spdlog::get("stdlogger")->warn("MediaInterface::try_connect uv_tcp_init {}, {}", status, uv_err_name(status));
    }

    uv_connect_t * connect_req = (uv_connect_t *)malloc(sizeof(uv_connect_t));
    connect_req->data = (that);

    status = uv_ip4_addr("127.0.0.1", 10020, &that->socket_address);
    if (status) {
        spdlog::get("stdlogger")->warn("MediaInterface::try_connect uv_ip4_addr {}, {}\n", status, uv_err_name(status));
    }

    status = uv_tcp_connect(connect_req, &that->client, (const struct sockaddr *)&that->socket_address, that->on_connect);
    if (status) {
        spdlog::get("stdlogger")->warn("MediaInterface::try_connect uv_tcp_connect {}, {}", status, uv_err_name(status));
    }
}

int MediaClientInterface::Start() {

    connectTimer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    connectTimer->data = (void *) this;
    uv_timer_init(uv_default_loop(), connectTimer);
    uv_timer_start(connectTimer, &try_connect, 500, 2000);

    return 0;
}

int MediaClientInterface::Stop() {
    return 0;
}

void MediaClientInterface::PostRequest(MediaRequest & request) {
    Send(clientConnection, request.toJson());
}

int MediaClientInterface::Send(uv_stream_t * stream, string message) {
    uv_buf_t * uvBuf = (uv_buf_t *)malloc(sizeof(uv_buf_t));
    uvBuf->base = (char *) malloc(message.length());
    memcpy((void *) uvBuf->base, message.c_str(), message.length());
    uvBuf->len = message.length();

    logger->info("MDC TX ===>>> {}", string(uvBuf->base, uvBuf->len));

    uv_write_t * req = (uv_write_t *) malloc(sizeof(uv_write_t));
    req->data = (void *) uvBuf;
    int error = uv_write(req, stream, uvBuf, 1, on_write);
    if (error) {
        logger->error("MediaClientInterface::Send uv_write returned {}, {}", error, uv_err_name(error));
    }

    return 0;

}