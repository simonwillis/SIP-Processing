//
// Created by Simon Willis on 07/07/2016.
//

#ifndef UDPSEND_SOCKETSERVER_H
#define UDPSEND_SOCKETSERVER_H

#include <uv.h>
#include <string>
#include <vector>
#include <map>

typedef void (* NewConnectionHandler)(uint32_t connId, const std::string remoteAddress, const int port);
typedef void (* LostConnectionHandler)(uint32_t connId);
typedef void (* SocketMessageHandler)(uint32_t connId, const char * buf, const size_t len);

class SocketServer {

public:

    SocketServer(int port, std::vector<uint8_t> messageDelimiter);
    ~SocketServer();

    void Start();
    void Stop();

    void setMessageDelimiter(std::vector<uint8_t> delimiter);
    void setMessageDelimiter(uint32_t connId, std::vector<uint8_t> delimiter);

    void setNewConnectionHandler(NewConnectionHandler newConnectionHandler);
    void setLostConnectionHandler(LostConnectionHandler lostConnectionHandler);
    void setSocketMessageHandler(SocketMessageHandler socketMessageHandler);

private:

    static void alloc_buffer(uv_handle_t * /*handle*/, size_t /*suggested_size*/, uv_buf_t * uvBuf);
    static void on_read(uv_stream_t * stream, ssize_t bytes_read, const uv_buf_t * uvBuf);
    static void on_write(uv_write_t * req, int status);
    static void on_connect(uv_stream_t * server, int status);
    static void on_read_close(uv_handle_t * handle);

    static uv_thread_t loop_thread;
    static void loopProcess(void * data);

    static uv_tcp_t server;
    static struct sockaddr_in serverAddress;

    uint32_t lastAllocatedConnectionId = 0;
    int port;
    std::vector<uint8_t> messageDelimiter;

    uint32_t getNextConnectionId() { return ++lastAllocatedConnectionId; };



    struct ConnectionData {
        uint32_t connId;
        void * data;
        uv_stream_t * stream;
    };

    std::map<uint32_t, ConnectionData> connections;

};


#endif //UDPSEND_SOCKETSERVER_H
