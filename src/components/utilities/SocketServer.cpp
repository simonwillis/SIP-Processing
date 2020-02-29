//
// Created by Simon Willis on 07/07/2016.
//

#include <string>
#include <cstring> // required for memcpy with some compilers, ignore the unused notification
#include <stdlib.h>
#include "SocketServer.h"
#include "../../../tools/RaiiLog.h"

uv_tcp_t SocketServer::server;
struct sockaddr_in SocketServer::serverAddress;
uv_thread_t SocketServer::loop_thread;

SocketServer::SocketServer(int port, std::vector<uint8_t> messageDelimiter)
        : port(port), messageDelimiter(messageDelimiter) {
    RaiiLog raiiLog = RaiiLog("SocketServer::CTOR");
}

SocketServer::~SocketServer() {
    RaiiLog raiiLog = RaiiLog("SocketServer::DTOR");
}

void SocketServer::Start() {
    RaiiLog raiiLog = RaiiLog("SocketServer::Start");

    uv_tcp_init(uv_default_loop(), &server);

    uv_ip4_addr("0.0.0.0", port, &serverAddress);

    uv_tcp_bind(&server, (struct sockaddr *)&serverAddress, 0);

    int error = uv_listen((uv_stream_t *)&server, 10, on_connect);

    uv_thread_create(&loop_thread, loopProcess, (void *)this);
}

void SocketServer::Stop() {
    RaiiLog raiiLog = RaiiLog("SocketServer::Stop\n");
    // trigger a halt here and wait for stop

    uv_thread_join(&loop_thread);
}

void SocketServer::setMessageDelimiter(std::vector<uint8_t> delimiter) {
    RaiiLog raiiLog = RaiiLog("SocketServer::setMessageDelimiter - general\n");
}

void SocketServer::setMessageDelimiter(uint32_t connId, std::vector<uint8_t> delimiter) {
    RaiiLog raiiLog = RaiiLog("SocketServer::setMessageDelimiter - on connection");
}

void SocketServer::setNewConnectionHandler(NewConnectionHandler newConnectionHandler) {
    RaiiLog raiiLog = RaiiLog("SocketServer::setNewConnectionHandler\n");
}

void SocketServer::setLostConnectionHandler(LostConnectionHandler lostConnectionHandler) {
    RaiiLog raiiLog = RaiiLog("SocketServer::setLostConnectionHandler\n");
}

void SocketServer::setSocketMessageHandler(SocketMessageHandler socketMessageHandler) {
    RaiiLog raiiLog = RaiiLog("SocketServer::setSocketMessageHandler\n");
}

void SocketServer::loopProcess(void * data) {
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void SocketServer::alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * uvBuf) {
    fprintf(stderr, "SocketServer::alloc_buffer\n");
    uvBuf->base = (char *)malloc(1024);
    uvBuf->len = 1024;

}

void SocketServer::on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * uvBuf) {
    fprintf(stderr, "SocketServer::on_read (byteCount=%zd]\n", bytes_read);

}

void SocketServer::on_write(uv_write_t * req, int status) {
    fprintf(stderr, "SocketServer::on_write\n");
}

void SocketServer::on_connect(uv_stream_t * stream, int status) {
    fprintf(stderr, "SocketServer::on_connect\n");

    SocketServer * that = (SocketServer *)stream->data;

    uv_tcp_t * client = new uv_tcp_t;

    int error = uv_tcp_init(uv_default_loop(), client);

    client->data = (void *) that;

    if (error) {
        fprintf(stderr, "on_connect uv_tcp_init failed with error %d %s\n", error, uv_err_name(error));
        delete client;
        return;
    }

    error = uv_accept(stream, (uv_stream_t *) client);

    if (error) {
        fprintf(stderr, "on_connect uv_accept failed with error %d %s\n", error, uv_err_name(error));
        delete client;
        return;
    }

    error = uv_read_start((uv_stream_t *) client, alloc_buffer, on_read);

    if (error) {
        fprintf(stderr, "on_connect uv_read_start failed with error %d %s\n", error, uv_err_name(error));
        delete client;
        return;
    }

}

void SocketServer::on_read_close(uv_handle_t * handle) {
    fprintf(stderr, "SocketServer::on_read_close");
}
