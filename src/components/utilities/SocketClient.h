//
// Created by Simon Willis on 21/02/2017.
//

#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <uv.h>

typedef void (* MessageHandler)(void * message, size_t size);

class SocketClient {



    static MessageHandler handler;
    static uv_stream_t * clientConnection;

    static void alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * buffer);
    static void on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * buffer);
    static void on_read_close(uv_handle_t * handle);
    static void on_write(uv_write_t * req, int status);
    static void try_connect(uv_timer_t * timer);

};


#endif //SOCKETCLIENT_H
