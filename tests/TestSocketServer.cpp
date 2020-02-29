//
// Created by Simon Willis on 07/07/2016.
//


#include <iostream>
#include "../src/components/utilities/SocketServer.h"

using namespace std;

void newConnectionHandler(uint32_t connId, const std::string remoteAddress, const int port) {
    fprintf(stderr, "New Connection [connId=%u] [address=%s] [port=%d]\n", connId, remoteAddress.c_str(), port);
}


int main(int argc, char * argv[]) {

    fprintf(stderr, "Work in progress\n");
    vector<uint8_t> delimiter = {10, 13, 10, 13};
    SocketServer socketServer(5062, delimiter);

    socketServer.setNewConnectionHandler(newConnectionHandler);

    socketServer.Start();

    char buf[80];
    cin.getline(buf, sizeof(buf));

    socketServer.Stop();
}
