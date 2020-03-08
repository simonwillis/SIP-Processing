//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPINTERFACE_H
#define CORE_SIPINTERFACE_H

#include <uv.h>

#include <unordered_map>
#include "messages/SipMessage.h"
#include "events/CallEvent.h"
#include "../utilities/buffers.h"
#include "messages/SipResponse.h"
#include "messages/SipRequest.h"
#include "SipParser.h"
#include "SipDialogueManager.h"
#include "messages/SipInviteRequest.h"
#include "messages/SipAckRequest.h"
#include "messages/SipByeRequest.h"
#include "messages/SipCancelRequest.h"
#include "messages/SipOptionsRequest.h"
#include "SipConnection.hpp"
#include "messages/SipSubscribeRequest.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>


typedef void (* CallEventHandler)(CallEvent * callEvent);

typedef struct media_stats {
    int packetsTransmitted;
    int packetsReceived;
    int packetsReceivedOutOfOrder;
    int rxPacketsLost;
    int badPacketsRx;
    int duplicatePacketsReceived;
    int discardedReceivedPackets;
    int jitterLast;
    int jitterMax;
    int jitterSum;
    int jitterMean;
} media_stats_t;

class SipInterface {

public:
    SipInterface(int port, CallEventHandler eventHandler);
    ~SipInterface();

public:

    static const int DefaultPort = 5060;
    static const int MaxActiveChannels = 1023;

public:
    std::string getMediaOffer(uint32_t channelId);
    int answerCall(uint32_t channelId, const char * tag, const char * contactField, std::string sdpAnswer);
    int startRecordingCall(uint32_t channelId, const char * filename);
    int stopRecordingCall(uint32_t channelId);
    int rejectCall(uint32_t channelId, int reasonCode, const char * reasonMessage);
    int clearCall(uint32_t channelId, int reasonCode, const char * reasonMessage);

    SipTransaction getTransaction(uint32_t channelId);

public:

    int Start(int port = 5060);
    int Stop();

private:
    void processMessage(SipMessage * sipMessage);
    void processSubscribe(SipSubscribeRequest * request);
    void processInvite(SipInviteRequest * sipMessage);
    void processAck(SipAckRequest * sipMessage);
    void processBye(SipByeRequest * sipMessage);
    void processCancel(SipCancelRequest * sipMessage);
    void processOptions(SipOptionsRequest * sipMessage);
    void processResponse(SipResponse * sipMessage);

    int sendMessage(const char *remoteSipAddress, int remoteSipPort, SipMessage::SipMessageTransportType transportType,
                    std::string responseMessage);

    uint32_t getNextConnectionId() { return ++lastConnectionId; }

private:

    static void onUdpSend(struct uv_udp_send_s *req, int status);
    static void onUdpRecv(struct uv_udp_s *handle, long int bytes_read, const struct uv_buf_t *uv_buffer,
                          const struct sockaddr *addr, unsigned int flags);
    static void alloc_buffer(struct uv_handle_s * handle, long unsigned int size, struct uv_buf_t* buf);

    static void onTcpConnection(uv_stream_t *server, int status);
    static void onTcpRecv(uv_stream_t *client, ssize_t nread, const uv_buf_t *uv_buffer);
    static void tcpOnWrite(uv_write_t *req, int status);

    static void sipUdpMessageHandler(SipMessage * sipMessage, void * callbackData);
    static void sipTcpMessageHandler(SipMessage * sipMessage, void * callbackData);

private:
    int port;
    static CallEventHandler handler;
    static uv_udp_t udpServerSocket;
    static uv_udp_t udpClientSocket;
    static struct sockaddr_in socket_address;

    static uv_tcp_t tcpServer;

    static SipDialogueManager dialogueManager;

    SipParser * parser;
    uint32_t lastConnectionId = 0;
    std::map<uint32_t, SipConnection *> connections;

    std::deque<int> availableChannelIds;

private:
    std::shared_ptr<spdlog::logger> logger;

};


#endif //CORE_SIPINTERFACE_H
