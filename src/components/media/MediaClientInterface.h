//
// Created by Simon Willis on 09/06/2016.
//

#ifndef MEDIA_MEDIASERVERINTERFACE_H
#define MEDIA_MEDIASERVERINTERFACE_H


#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "messages/MediaResponse.h"
#include "messages/MediaRequest.h"

#include <uv.h>

typedef void (* MediaResponseHandler)(MediaResponse * response);

class MediaClientInterface {

public:

    MediaClientInterface(std::string serverAddress, int serverPort, MediaResponseHandler handler);
    int Start();
    int Stop();
    void PostRequest(MediaRequest & request);

private:

    uv_tcp_t client;

    struct sockaddr_in socket_address;

    static void on_connect(uv_connect_t *req, int status);

private:

    const std::string remoteAddress;
    const int remotePort;

    uv_timer_t * connectTimer;

private:

    int Send(uv_stream_t * stream, std::string message);

private:

    static MediaResponseHandler handler;
    static uv_stream_t * clientConnection;

    static void alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * buffer);
    static void on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * buffer);
    static void on_read_close(uv_handle_t * handle);
    static void on_write(uv_write_t * req, int status);
    static void try_connect(uv_timer_t * timer);

    std::shared_ptr<spdlog::logger> logger;

};


#endif //MEDIA_MEDIASERVERINTERFACE_H
