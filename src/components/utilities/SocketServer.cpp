//
// Created by Simon Willis on 07/07/2016.
//

#include <string>
#include <cstring> // required for memcpy with some compilers, ignore the unused notification
#include <stdlib.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include "SocketServer.h"

uv_tcp_t SocketServer::server;
struct sockaddr_in SocketServer::serverAddress;
uv_thread_t SocketServer::loop_thread;

SocketServer::SocketServer(int port, std::vector<uint8_t> messageDelimiter)
        : port(port), messageDelimiter(messageDelimiter) {
}

SocketServer::~SocketServer() {
}

void SocketServer::Start() {

    uv_tcp_init(uv_default_loop(), &server);

    uv_ip4_addr("0.0.0.0", port, &serverAddress);

    uv_tcp_bind(&server, (struct sockaddr *)&serverAddress, 0);

    int error = uv_listen((uv_stream_t *)&server, 10, on_connect);

    uv_thread_create(&loop_thread, loopProcess, (void *)this);
}

void SocketServer::Stop() {
    // trigger a halt here and wait for stop

    uv_thread_join(&loop_thread);
}

void SocketServer::setMessageDelimiter(std::vector<uint8_t> delimiter) { }

void SocketServer::setMessageDelimiter(uint32_t connId, std::vector<uint8_t> delimiter) { }

void SocketServer::setNewConnectionHandler(NewConnectionHandler newConnectionHandler) { }

void SocketServer::setLostConnectionHandler(LostConnectionHandler lostConnectionHandler) { }

void SocketServer::setSocketMessageHandler(SocketMessageHandler socketMessageHandler) { }

void SocketServer::loopProcess(void * data) {
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void SocketServer::alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * uvBuf) {
    uvBuf->base = (char *)malloc(1024);
    uvBuf->len = 1024;
}

void SocketServer::on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * uvBuf) { }

void SocketServer::on_write(uv_write_t * req, int status) { }

void SocketServer::on_connect(uv_stream_t * stream, int status) {

    SocketServer * that = (SocketServer *)stream->data;

    uv_tcp_t * client = new uv_tcp_t;

    int error = uv_tcp_init(uv_default_loop(), client);

    client->data = (void *) that;

    if (error) {
        spdlog::get("stdlogger")->error( "on_connect uv_tcp_init failed with error {}, {}", error, uv_err_name(error));
        delete client;
        return;
    }

    error = uv_accept(stream, (uv_stream_t *) client);

    if (error) {
        spdlog::get("stdlogger")->error("on_connect uv_accept failed with error %d %s\n", error, uv_err_name(error));
        delete client;
        return;
    }

    error = uv_read_start((uv_stream_t *) client, alloc_buffer, on_read);

    if (error) {
        spdlog::get("stdlogger")->error("on_connect uv_read_start failed with error %d %s\n", error, uv_err_name(error));
        delete client;
        return;
    }
}

void SocketServer::on_read_close(uv_handle_t * handle) {
    spdlog::get("stdlogger")->debug("SocketServer::on_read_close");
}
