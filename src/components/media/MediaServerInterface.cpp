//
// Created by Simon Willis on 10/06/2016.
//

#include "MediaServerInterface.h"
#include <cstring> // required for memcpy with some compilers, ignore the unused notification
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

using namespace std;



MediaRequestHandler MediaServerInterface::handler;
uv_tcp_t MediaServerInterface::server;
struct sockaddr_in MediaServerInterface::serverAddress;
uv_stream_t * MediaServerInterface::connectedStream;

void MediaServerInterface::alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * buffer) {
    buffer->base = (char *)malloc(16192);
    buffer->len = 16192;
}

void MediaServerInterface::on_read_close(uv_handle_t * handle) {
    uv_stream_t * stream = (uv_stream_t *) handle;

    //TODO add removeStreamFromCollection(stream);

}

void MediaServerInterface::on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * buffer) {
    if (bytes_read < 0) {
        spdlog::get("stdlogger")->error("MediaServerInterface Stream Handler. Read error [errorCode={}] [errorMessage={}]", bytes_read, uv_err_name(bytes_read));
        uv_read_stop(stream);
        uv_close((uv_handle_t *)stream, on_read_close);
        return;
    }

    string message = string(buffer->base, bytes_read);

    free((void *)buffer->base);

    spdlog::get("stdlogger")->debug("MDC RX <<<===\n{}", message);

    MediaMessage * msg = MediaMessage::build_message(message.c_str());

    if (msg) {
        if (msg->getType() == MediaMessage::Type::REQUEST) {
            handler((MediaRequest *)msg, stream);
        } else {
            spdlog::get("stdlogger")->warn("MediaServerInterface::on_read received a '{}' message, expected a 'request'", msg->getTypeStr());
        }

    } else {
        spdlog::get("stdlogger")->warn("MediaServerInterface::on_read failed to build valid request [json={}]", message);
    }

}

void MediaServerInterface::on_write(uv_write_t * req, int status) {
    uv_buf_t * uvBuf = (uv_buf_t *)req->data;
    if (status) {
        spdlog::get("stdlogger")->debug("MediaServerInterface - Write completed with status {}", status);
        return;
    }
    if (req->data == NULL) {
        spdlog::get("stdlogger")->warn("on_write was passed empty data on, ignoring");
        return;
    }

    if (uvBuf) {
        if (uvBuf->base == NULL) {
            spdlog::get("stdlogger")->warn("on_write uvBuf base is null, ignoring");
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

void MediaServerInterface::on_connect(uv_stream_t * stream, int status) {
    spdlog::get("stdlogger")->debug("MediaServerInterface::on_connect");
    uv_tcp_t * client = new uv_tcp_t;

    int error = uv_tcp_init(uv_default_loop(), client);

    if (error) {
        spdlog::get("stdlogger")->error("on_controller_connect uv_tcp_init failed with error {}, {}", error, uv_strerror(error));
        delete client;
        return;
    }

    error = uv_accept(stream, (uv_stream_t *) client);

    if (error) {
        spdlog::get("stdlogger")->error("on_controller_connect uv_accept failed with error {}.{}", error, uv_strerror(error));
        delete client;
        return;
    }

    error = uv_read_start((uv_stream_t *) client, alloc_buffer, on_read);

    if (error) {
        spdlog::get("stdlogger")->error("on_controller_connect uv_read_start failed with error {}, {}", error, uv_strerror(error));
        delete client;
        return;
    }

    connectedStream = stream;

    // connectedStreams[stream] = client;

}

MediaServerInterface::MediaServerInterface(int port, MediaRequestHandler requestHandler)
        : port(port) {
    handler = requestHandler;
}

int MediaServerInterface::Start() {

    uv_tcp_init(uv_default_loop(), &server);

    uv_ip4_addr("0.0.0.0", port, &serverAddress);

    uv_tcp_bind(&server, (struct sockaddr *)&serverAddress, 0);

    int error = uv_listen((uv_stream_t *)&server, 10, on_connect);

    return (error == 0);
}

int MediaServerInterface::Stop() {
    return 0;
}

int MediaServerInterface::PostResponse(uv_stream_t * stream, MediaResponse & response) {
    return Send(stream, response.toJson());
}

int MediaServerInterface::Send(uv_stream_t * stream, string message) {

    int error = 0;

    uv_buf_t * uvBuf = (uv_buf_t *)malloc(sizeof(uv_buf_t));
    uvBuf->base = (char *) malloc(message.length());
    memcpy((void *) uvBuf->base, message.c_str(), message.length());
    uvBuf->len = message.length();

    spdlog::get("stdlogger")->debug("MDC TX ===>>>\n{}", string(uvBuf->base, uvBuf->len));



    uv_write_t * req = (uv_write_t *) malloc(sizeof(uv_write_t));
    req->data = (void *) uvBuf;
    error = uv_write(req, stream, uvBuf, 1, on_write);
    if (error) {
        spdlog::get("stdlogger")->error("MediaServerInterface::Send uv_write returned {}, {}", error, uv_err_name(error));
    }

    return error;

}
