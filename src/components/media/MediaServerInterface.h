//
// Created by Simon Willis on 10/06/2016.
//

#ifndef MEDIA_MEDIASERVERINTERFACE_H
#define MEDIA_MEDIASERVERINTERFACE_H

#include <string>
#include "messages/MediaRequest.h"
#include "messages/MediaResponse.h"

#include <uv.h>

typedef void (* MediaRequestHandler)(MediaRequest * request, uv_stream_t * stream);

class MediaServerInterface {

public:

    MediaServerInterface(int port, MediaRequestHandler requestHandler);
    int Start();
    int Stop();
    int PostResponse(uv_stream_t * stream, MediaResponse & response);

private:

    static void alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * buffer);
    static void on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * buffer);
    static void on_write(uv_write_t * req, int status);
    static void on_connect(uv_stream_t * server, int status);
    static void on_read_close(uv_handle_t * handle);

private:

    MediaServerInterface() {};

    int Send(uv_stream_t * stream, std::string message);

private:

    int port;
    static MediaRequestHandler handler;
    static uv_tcp_t server;
    static struct sockaddr_in serverAddress;
    static uv_stream_t * connectedStream;

};


#endif //MEDIA_MEDIASERVERINTERFACE_H
