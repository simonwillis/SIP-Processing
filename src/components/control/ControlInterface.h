//
// Created by Simon Willis on 20/05/2016.
//

#ifndef CORE_CONTROLINTERFACE_H
#define CORE_CONTROLINTERFACE_H

#include <string>

#include <uv.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "requests/Request.h"
#include "notifications/Notification.hpp"
#include "../sip/headers/SipHeader.h"

typedef std::map<uv_stream_t *, uv_tcp_t *> stream_collection_t;

typedef void (* MessageHandler)(Request * request, uv_stream_t * stream);


class ControlInterface {

public:
    ControlInterface(int port, MessageHandler requestHandler);
    ~ControlInterface();

    int Start();
    int Stop();

    void PostNotification(uv_stream_t * stream, Notification & notification);

public:
    static const int DefaultPort = 10010;

private:

    static stream_collection_t connectedStreams;
    static void removeStreamFromCollection(uv_stream_t * stream);

private:

    static void alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * buffer);
    static void on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * buffer);
    static void on_write(uv_write_t * req, int status);
    static void on_connect(uv_stream_t * server, int status);
    static void on_read_close(uv_handle_t * handle);


private:

    ControlInterface() {};

    int Send(uv_stream_t * stream, std::string message);

private:

    int port;
    static MessageHandler handler;
    static uv_tcp_t server;
    static struct sockaddr_in serverAddress;
    static uv_stream_t * connectedStream;

    std::shared_ptr<spdlog::logger> logger;

};


#endif //CORE_CONTROLINTERFACE_H
