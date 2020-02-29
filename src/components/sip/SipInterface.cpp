//
// Created by Simon Willis on 23/05/2016.
//

//#define BOOST_UUID_NO_SIMD // Stop the compiler optimising based on the chip the code is compiled on. AWS supports SIMD on its processors


#include <string>
#include <vector>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>


#include "SipInterface.h"
#include "events/CallNewCallEvent.h"
#include "events/CallAnsweredEvent.h"
#include "events/CallClearedEvent.h"
#include "events/CallMediaRecordingStartedEvent.h"
#include "../utilities/util.hpp"
#include "../../../tools/RaiiLog.h"
#include "messages/SipSubscribeRequest.h"

using namespace std;

CallEventHandler SipInterface::handler;
uv_udp_t SipInterface::udpServerSocket;
uv_udp_t SipInterface::udpClientSocket;
uv_tcp_t SipInterface::tcpServer;
struct sockaddr_in SipInterface::socket_address;
SipDialogueManager SipInterface::dialogueManager;

SipInterface::SipInterface(int port, CallEventHandler eventHandler)
        : port(port) {

    RaiiLog raiiLog = RaiiLog("SipInterface::CTOR");

    handler = eventHandler;

    // We can use a single parser object here as we received UDP messages in their totality
    // SIP Messages on TCP can have long running connections that handle blocks of data.
    // in the case of TCP we set up a sip parser per TCP Connection.
    parser = new SipParser(sipUdpMessageHandler, this);
}

SipInterface::~SipInterface() {
    RaiiLog raiiLog = RaiiLog("SipInterface::DTOR");

    //TODO Implement SipInterface DTOR
    fprintf(stderr, "SipInterface::DTOR\n");
}

void SipInterface::processMessage(SipMessage * sipMessage) {
    RaiiLog raiiLog = RaiiLog("SipInterface::processMessage");

    SipTransaction * transaction = nullptr;

    if (! sipMessage) {
        fprintf(stderr, "SipInterface cannot process empty message\n");
        return;
    }



//    SipDialogue * dialogue = dialogueManager.retrieveDialogue(sipMessage->getChannelId());
//    if (dialogue) {

    transaction = dialogueManager.updateTransaction(sipMessage);

    if (! transaction) {
        fprintf(stderr, "\nSipInterface::processMessage: updateTransaction returned null\n");
        return;
    }
    uint32_t channelId = transaction->channelId;


    if (sipMessage->isRequest()) {
            SipRequest * sipRequest = (SipRequest *)sipMessage;
            fprintf(stderr, "\nSIP RX <<<=== [channelId=%u] [method=%s]\n%s\n", channelId, SipMessage::getTypeStr(sipRequest->getRequestType()), sipRequest->message);
            SipMessage::SipMessageType requestType = ((SipRequest *)sipMessage)->getRequestType();
            switch (requestType) {
                case SipMessage::SipMessageType::INVITE:
                    processInvite((SipInviteRequest *) sipMessage); break;
                case SipMessage::SipMessageType::ACK:
                    processAck((SipAckRequest *) sipMessage); break;
                case SipMessage::SipMessageType::BYE:
                    processBye((SipByeRequest *) sipMessage); break;
                case SipMessage::SipMessageType::CANCEL:
                    processCancel((SipCancelRequest *) sipMessage); break;
                case SipMessage::SipMessageType::OPTIONS:
                    processOptions((SipOptionsRequest *) sipMessage); break;
                case SipMessage::SipMessageType::SUBSCRIBE:
                    processSubscribe((SipSubscribeRequest *) sipMessage); break;
                default:
                    fprintf(stderr, "Not currently handling %s Request in inbound SIP stream handler [channel=%d]\n",
                            sipMessage->getSubTypeDescriptionString(), channelId);
                    break;
            }
        } else {
            SipMessage::SipMessageType  responseType = ((SipResponse *)sipMessage)->getResponseType();
            switch (responseType) {
                default:
                    fprintf(stderr, "Not currently handling %s Response in inbound SIP stream handler [channel=%d]\n",
                            sipMessage->getSubTypeDescriptionString(), channelId);
                    break;
            }
        }
//    } else {
//        fprintf(stderr, "Failed to locate dialogue associated with incoming %s %s message [channel=%d]\n",
//                sipMessage->getSubTypeDescriptionString(), sipMessage->getTypeDescriptionString(), channelId);
//    }
}

void  SipInterface::sipUdpMessageHandler(SipMessage * sipMessage, void * callbackData) {
    RaiiLog raiiLog = RaiiLog("SipInterface::sipUdpMessageHandler");
    SipInterface * that = (SipInterface *)callbackData;
    fprintf(stderr, "SIP (UDP) Event Handler triggered with [%s %s] [UDP]\n",
            sipMessage->getSubTypeDescriptionString(), sipMessage->getTypeDescriptionString());

    that->processMessage(sipMessage);
    return;
}

void  SipInterface::sipTcpMessageHandler(SipMessage * sipMessage, void * callbackData) {
    RaiiLog raiiLog = RaiiLog("SipInterface::sipTcpMessageHandler");
    SipConnection * sipConnection = (SipConnection *) callbackData;

    fprintf(stderr, "SIP (TCP) Event Handler triggered with sipMessage [%s %s] [hasBody=%s] [TCP] [connId=%ul]\n",
            sipMessage->getSubTypeDescriptionString(), sipMessage->getTypeDescriptionString(),
            sipMessage->hasBody() ? "true":"false",
            (sipConnection ? sipConnection->connectionId : 0));

    if (sipMessage->hasBody(SipMessageBody::BodyType::SDP)) {
        fprintf(stderr, "SipInterface::sipTcpMessageHandler: message has an SDP body\n");
    }

    SipInterface * that = (SipInterface *) sipConnection->data;

    that->processMessage(sipMessage);
    return;
}

void SipInterface::onUdpSend(uv_udp_send_t *req, int status) {
    RaiiLog raiiLog = RaiiLog("SipInterface::onUdpSend");

    if (status) {
        fprintf(stderr, "SipInterface::onUdpSend called with status %d\n", status);
    }

}

void SipInterface::onUdpRecv(uv_udp_t *handle, long int bytes_read, const struct uv_buf_t *uv_buffer,
                             const struct sockaddr *addr, unsigned int flags) {
    RaiiLog raiiLog = RaiiLog("SipInterface::onUdpRecv");

    if (bytes_read < 0) {

        fprintf(stderr, "SIP UDP Stream Handler. Read error [errorCode=%ld] [errorMessage=%s]\n", bytes_read, uv_err_name(bytes_read));

    } else if (bytes_read > 0) {

        //fprintf(stderr, "SipInterface::onUdpRecv Received %ld bytes\n", bytes_read);

        char * p;
        long int i;
        for (i = 0, p = uv_buffer->base; i < bytes_read; ++i, p++) {
            printChar(*p);
        }

        size_t count;
        char buf[4096];
        MessageBuffer messageBuffer(4096);

        // the handle's data field holds the reference to the instance of the interface (set on call to Start())
        SipInterface * that = (SipInterface *)handle->data;

        messageBuffer.push(uv_buffer->base, (size_t)bytes_read);

        // UDP SIP Messages do not neccesarily terminate with a double CRLF, typically just a single
        do {
            count = messageBuffer.pull(buf, sizeof(buf), SipParser::messageDelimiter);
            if (count == 0 && messageBuffer.getStoredByteCount()) {
                count = messageBuffer.pull(buf, sizeof(buf), messageBuffer.getStoredByteCount());
            }

            //fprintf(stderr, "SipInterface::onUdpRecv pushing %zu bytes to parser\n", count);
            that->parser->loadMessageData(buf, count);
        } while (messageBuffer.getStoredByteCount());

    }

    //fprintf(stderr, "SipInterface::onUdpRecv completing\n");

    if (uv_buffer->len) {
        free(uv_buffer->base);
    }

}

void SipInterface::onTcpRecv(uv_stream_t *client, ssize_t bytes_read, const uv_buf_t *uv_buffer) {
    RaiiLog raiiLog = RaiiLog("SipInterface::onTcpRecv");
    fprintf(stderr, "SipInterface::onTcpRecv: [bytes_read=%zu]\n", bytes_read);

    if (bytes_read == UV_EOF) {
        // XXX: this is actually a bug, since client writes could back up (see ./test-client.sh) and therefore
        // some chunks to be written be left in the pipe.
        // Closing in that case results in: [ECANCELED: operation canceled]
        // Not closing here avoids that error and a few more chunks get written.
        // But even then not ALL of them make it through and things get stuck.
        uv_close((uv_handle_t*) client, NULL);

        fprintf(stderr, "SipInterface::onTcpRecv: closed client connection\n");

    } else if (bytes_read > 0) {
        //fprintf(stderr, "SipInterface::onTcpRecv: %ld bytes read\n", bytes_read);
//        for (size_t i = 0; i < bytes_read; ++i) {
//            printChar(uv_buffer->base[i]);
//        }

        SipConnection * sipConnection = (SipConnection *) client->data;
        SipInterface * that = (SipInterface *)sipConnection->data;

        if (that == 0) {
            fprintf(stderr, "SipInterface::onTcpRecv: forgot to set the data value to 'this'\n");
            return;
        }

        fprintf(stderr, "SipInterface::onTcpRecv: pushing %ld bytes onto message buffer\n", bytes_read);
        sipConnection->messageBuffer->push(uv_buffer->base, (size_t)bytes_read);
        //fprintf(stderr, "SipInterface::onTcpRecv: pushing extra crlf\n");
        //sipConnection->messageBuffer->push("\r\n", 2);


        // The message buffer drains out on delimiters, SIP Messages are always delimited with "\r\n\r\n"
        // The SipParser spits out messages when it has enough data.
        char buf[1024];
        size_t count;

        do {
            if (sipConnection->pendingMessageBytes) {
                fprintf(stderr, "SipInterface::onTcpRecv: sipConnection pending %zu bytes\n", sipConnection->pendingMessageBytes);
                count = sipConnection->messageBuffer->pull(buf, sizeof(buf), sipConnection->pendingMessageBytes);
            } else {
                count = sipConnection->messageBuffer->pull(buf, sizeof(buf), SipParser::messageDelimiter);
            }
            fprintf(stderr, "SipInterface::onTcpRecv: Pulled %zu bytes from buffer\n", count);
//            fprintf(stderr, "SipInterface::onTcpRecv: %s\n", string(buf, count).c_str());
            if (count) {
                sipConnection->pendingMessageBytes = sipConnection->sipParser->loadMessageData(buf, count);
                fprintf(stderr, "SipInterface::onTcpRecv: Load Message returned pending byte count %ld\n", sipConnection->pendingMessageBytes);
            }
        } while (count > 0);
        fprintf(stderr, "SipInterface::onTcpRecv: finished pulling from buffer\n");

    }
    if (bytes_read == 0) free(uv_buffer->base);
}

void SipInterface::onTcpConnection(uv_stream_t *server, int status) {
    RaiiLog("SipInterface::onTcpConnection");

    if (status) {
        fprintf(stderr, "onTcpConnection error %d (%s)\n", status, uv_err_name(status));
        return;
    }

    SipInterface * that = (SipInterface *) server->data;

    uint32_t connectionId = that->getNextConnectionId();

    SipConnection * sipConnection = new SipConnection(connectionId, that, server, sipTcpMessageHandler);


    fprintf(stderr, "Allocated new connectionId %u\n", connectionId);

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(uv_default_loop(), client);

    client->data = (void *) sipConnection;

    int result = uv_accept(server, (uv_stream_t*) client);
    if (result) {
        fprintf(stderr, "Failed to accept connection error=%d (%s)\n", result, uv_err_name(result));
        uv_close((uv_handle_t*) client, NULL);
    } else {
        uv_read_start((uv_stream_t *) client, alloc_buffer, onTcpRecv);
    }
}

void SipInterface::alloc_buffer(struct uv_handle_s* handle, long unsigned int size, struct uv_buf_t* buf) {
    RaiiLog raiiLog = RaiiLog("SipInterface::alloc_buffer");

    long unsigned int bufSize = 8192;

    buf->base = (char *)malloc(bufSize);
    buf->len = bufSize;
}

int SipInterface::Start(int port) {
    RaiiLog raiiLog = RaiiLog("SipInterface::Start");

    int result, status;

    fprintf(stderr, "Starting SIP on port %d\n", port);

    uv_udp_init(uv_default_loop(), &udpServerSocket);
    udpServerSocket.data = (void *) this;
    struct sockaddr_in udp_recv_addr;
    uv_ip4_addr("0.0.0.0", port, &udp_recv_addr);
    status = uv_udp_bind(&udpServerSocket, (const struct sockaddr *)&udp_recv_addr, UV_UDP_REUSEADDR);
    if (status) {
        fprintf(stderr, "UDP bind server returned %d, %s, %s\n", status, uv_err_name(status), uv_strerror(status));
    }
    uv_udp_recv_start(&udpServerSocket, alloc_buffer, onUdpRecv);


    uv_tcp_init(uv_default_loop(), &tcpServer);
    struct sockaddr_in tcp_recv_addr;
    result = uv_ip4_addr("0.0.0.0", port, &tcp_recv_addr);
    if (result) {

        fprintf(stderr, "Failed to set up tcp server address in start error = %d (%s)\n", result, uv_err_name(result));

    } else {

        uv_tcp_bind(&tcpServer, (struct sockaddr *) &tcp_recv_addr, 0);
        tcpServer.data = (void *) this;

        fprintf(stderr, "Starting listen on TCP port %d\n", port);
        result = uv_listen((uv_stream_t *) &tcpServer, 64, onTcpConnection);
        if (result) fprintf(stderr, "listen error %d (%s)", result, uv_err_name(result));
    }

//    uv_udp_init(uv_default_loop(), &udpClientSocket);
//    struct sockaddr_in udp_send_addr;
//    uv_ip4_addr("192.168.0.4", 5061, &udp_send_addr);
//    //udp_send_addr.sin_port = 5061;
//    status = uv_udp_bind(&udpClientSocket, (const struct sockaddr *)&udp_send_addr, UV_UDP_REUSEADDR);
//    if (status) {
//        fprintf(stderr, "UDP bind client returned %d, %s, %s\n", status, uv_err_name(status), uv_strerror(status));
//    }

//    uv_udp_init(uv_default_loop(), &udpClientSocket);
//    uv_ip4_addr("0.0.0.0", port, &udp_send_addr);


    return 0;
}


void SipInterface::tcpOnWrite(uv_write_t *req, int status) {
    RaiiLog raiiLog = RaiiLog("SipInterface::tcpOnWrite");

    uv_req_t* wr = (uv_req_t*) req;

    free(wr);
}

int SipInterface::Stop() {
    RaiiLog raiiLog = RaiiLog("SipInterface::Stop");

    //TODO Implement SipInterface::Stop()
    fprintf(stderr, "SipInterface::Stop Not Implemented Yet\n");
    return 0;
}

std::string SipInterface::getMediaOffer(uint32_t channelId) {
    RaiiLog raiiLog = RaiiLog("SipInterface::getMediaOffer");
    fprintf(stderr, "SipInterface::getMediaOffer [channelId=%u]\n", channelId);
    SipDialogue * dialogue = dialogueManager.retrieveDialogue(channelId);
    if (dialogue) {
        SipTransaction * transaction = dialogue->getCurrentTransaction();
        if (transaction) {
            fprintf(stderr, "SipInterface::getMediaOffer [channelId=%u] [offer=%s]\n", channelId, transaction->sdpOffer.c_str());
            return transaction->sdpOffer;
        } else {
            fprintf(stderr, "SipInterface::getMediaOffer [channelId=%u] No transaction found\n", channelId);
        }
    } else {
        fprintf(stderr, "SipInterface::getMediaOffer [channelId=%u] No dialogue found\n", channelId);
    }

    return std::string();
}

int SipInterface::answerCall(uint32_t channelId, const char * tag, const char * contactField, std::string sdpAnswer) {
    RaiiLog raiiLog = RaiiLog("SipInterface::answerCall");

    fprintf(stderr, "SipInterface::answerCall TBC [channelId=%d] [tag=%s] [contact=%s]\n", channelId, tag, contactField);
    SipDialogue * dialogue = dialogueManager.retrieveDialogue(channelId);
    if (! dialogue) {
        fprintf(stderr, "SipInterface::answerCall failed to locate dialogue for channel [channelId=%d]\n", channelId);
        return 0;
    }

    fprintf(stderr, "Answering call via %s (TBC)\n", SipMessage::getTransportTypeStr(dialogue->getTransportType()));

    SipTransaction * transaction = dialogue->getCurrentTransaction();

    if (! transaction ) {
        fprintf(stderr, "SipInterface::answerCall no active transaction for channel [channelId=%u]\n", channelId);
    } else {
        fprintf(stderr, "Answering call. Have a transaction\n");
        if (transaction->getType() != SipMessage::SipMessageType::INVITE) {
            fprintf(stderr, "SipInterface::answerCall cannot complete should be 'INVITE' have %s\n", transaction->toString().c_str());
        }
        if (! transaction->sipRequest) {

            fprintf(stderr, "Transaction does not have a sipRequest defined\n");
            return 1;
        }

        SipInviteRequest * sipInviteRequest = (SipInviteRequest *) transaction->sipRequest;
        fprintf(stderr, "Answering call, building response string \n");
        string responseString = sipInviteRequest->buildAnswerResponse(dialogue->getResponseTag(), sdpAnswer);
        fprintf(stderr, "SipInterface::answerCall prepared response message as\n%s\n", responseString.c_str());
        sendMessage(transaction->sipRequest->getRemoteAddress().c_str(), transaction->sipRequest->getRemotePort(), transaction->sipRequest->getTransportType(),
                    responseString);
    }

    return 0;
}

int SipInterface::startRecordingCall(uint32_t channelId, const char * filename) {
    RaiiLog raiiLog = RaiiLog("SipInterface::startRecordingCall");

    //fprintf(stderr, "SipInterface::startRecordingCall TBC [channelId=%d]\n", channelId);

    SipDialogue * session = dialogueManager.retrieveDialogue(channelId);
    if (! session) {
        return -1;
    }

    //TODO Initiate recording process
    session->setRecording(true);

    CallMediaRecordingStartedEvent event = CallMediaRecordingStartedEvent(session->getChannelId(), session->getCallId().c_str(), true, 0, "OK");
    handler(&event);

    return 0;
}

int SipInterface::stopRecordingCall(uint32_t channelId) {
    RaiiLog raiiLog = RaiiLog("SipInterface::stopRecordingCall");

    //fprintf(stderr, "SipInterface::stopRecordingCall TBC [channelId=%d]\n", channelId);
    return 0;
}

int SipInterface::rejectCall(uint32_t channelId, int reasonCode, const char * reasonMessage) {
    RaiiLog raiiLog = RaiiLog("SipInterface::rejectCall");

    fprintf(stderr, "SipInterface::rejectCall TBC [channelId=%d]\n", channelId);
    return 0;
}

int SipInterface::clearCall(uint32_t channelId, int reasonCode, const char * reasonMessage) {
    RaiiLog raiiLog = RaiiLog("SipInterface::clearCall");

    fprintf(stderr, "SipInterface::clearCall TBC [channelId=%d]\n", channelId);
    return 0;
}


void SipInterface::processSubscribe(SipSubscribeRequest * request) {
    sendMessage(request->getRemoteAddress().c_str(),
                request->getRemotePort(),
                request->getTransportType(),
                request->buildSubscriptionAcceptedResponse("subscription"));
}

void SipInterface::processInvite(SipInviteRequest * request) {
    RaiiLog("SipInterface::processInvite");

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());

    if (! dialogue) {
        fprintf(stderr, "SipInterface::processInvite Session not set against request, aborting\n");
        return;
    } else {
        fprintf(stderr, "SipInterface: processInvite channel=%u callid=%s\n", dialogue->getChannelId(), dialogue->getCallId().c_str());
    }

    dialogue->setState(SipDialogue::DialogueState::INCOMING);

    sendMessage(request->getRemoteAddress().c_str(),
                request->getRemotePort(),
                request->getTransportType(),
                request->buildTryingResponse(dialogue->getResponseTag()));

    CallNewCallEvent event = CallNewCallEvent(dialogue->getChannelId(), request->getCallId(),
                                              dialogue->getResponseTag(), request->getTo(), request->getFrom(),
                                              request->getHeaders(), request->getBodies());

    handler(&event);
}

void SipInterface::processAck(SipAckRequest * request) {
    RaiiLog raiiLog = RaiiLog("SipInterface::processAck");

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());
    if (! dialogue) {
        fprintf(stderr, "SipInterface::processAck Session not set against request, aborting\n");
        return;
    }

    if (dialogue->getState() == SipDialogue::DialogueState::INCOMING) {
        dialogue->setState(SipDialogue::DialogueState::CONNECTED);
        CallAnsweredEvent event = CallAnsweredEvent(dialogue->getChannelId(), request->getCallId());
        handler(&event);
    }
}

void SipInterface::processBye(SipByeRequest * request) {
    RaiiLog raiiLog = RaiiLog("SipInterface::processBye");

    fprintf(stderr, "SipInterface::processBye\n");

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());
    if (! dialogue) {
        fprintf(stderr, "SipInterface::processBye Session not set against request, aborting\n");
        return;
    }

    if (dialogue->getState() == SipDialogue::DialogueState::CONNECTED) {

        if (dialogue->isRecording()) {
            CallMediaRecordingStartedEvent event = CallMediaRecordingStartedEvent(request->getChannelId(), request->getCallId().c_str(), false, 0, "OK");
            handler(&event);
        }

        dialogue->setState(SipDialogue::DialogueState::CLEARED);

        sendMessage(request->getRemoteAddress().c_str(), request->getRemotePort(),
                    request->getTransportType(),
                    request->buildOkResponse(dialogue->getResponseTag()));

        CallClearedEvent event = CallClearedEvent(dialogue->getChannelId(), request->getCallId());
        handler(&event);

    } else if (dialogue->getState() == SipDialogue::DialogueState::CLEARING) {

        dialogue->setState(SipDialogue::DialogueState::CLEARED);

        CallClearedEvent event = CallClearedEvent(dialogue->getChannelId(), request->getCallId());
        handler(&event);
    }
}

void SipInterface::processCancel(SipCancelRequest * request) {
    RaiiLog raiiLog = RaiiLog("SipInterface::processCancel");

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());
    if (! dialogue) {
        fprintf(stderr, "SipInterface::processCancel Session not set against request, aborting\n");
        return;
    }

    if (dialogue->getState() == SipDialogue::DialogueState::INCOMING) {
        CallClearedEvent event = CallClearedEvent(dialogue->getChannelId(), request->getCallId(), 487);
        handler(&event);
    }
}

void SipInterface::processOptions(SipOptionsRequest * request) {
    RaiiLog raiiLog = RaiiLog("SipInterface::processOptions");

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());
    if (! dialogue) {
        fprintf(stderr, "SipInterface::processOptions Session not set against request, aborting\n");
        return;
    }

    //TODO Fill in functionality for basic OPTIONS response
    //TODO Set up and send an options event to the controller

}

void SipInterface::processResponse(SipResponse * response) {
    RaiiLog raiiLog = RaiiLog("SipInterface::processResponse");

    //TODO Implement SipInterface::processResponse
    fprintf(stderr, "SipInterface::processResponse Not Implemented Yet\n");
    return;
}

SipTransaction SipInterface::getTransaction(uint32_t channelId) {
    RaiiLog raiiLog = RaiiLog("SipInterface::getTransaction");

    //TODO Implement SipInterface::getTransaction
    fprintf(stderr, "SipInterface::getTransaction Not Implemented Yet\n");

    return SipTransaction();
}

int SipInterface::sendMessage (
                    const char *remoteSipAddress,
                    int remoteSipPort,
                    SipMessage::SipMessageTransportType transportType,
                    std::string responseMessage) {

    RaiiLog raiiLog = RaiiLog("SipInterface::sendMessage");

    int status;
    uv_buf_t *buf;
    uv_udp_send_t *req;
    struct sockaddr_in destinationAddress;
    int spacer = 0;

    int destPort = remoteSipPort;

//    if (remoteSipPort != 5060) fprintf(stderr, "SipInterface::sendMessage: forcing remote port from %d to 5060\n", remoteSipPort);
//    remoteSipPort = 5060;

    fprintf(stderr, "\nSIP TX ===>>>\n%s:%s:%d\n%s\n",SipMessage::getTransportTypeStr(transportType), remoteSipAddress, remoteSipPort, responseMessage.c_str());

    //fprintf(stderr, "Message to go is %lu bytes\n", responseMessage.size());
    uv_ip4_addr(remoteSipAddress, destPort, &destinationAddress);
    //uv_ip4_addr(remoteSipAddress, 5060, &destinationAddress);

    buf = (uv_buf_t *)malloc(sizeof(uv_buf_t));
    buf->len = responseMessage.size() + spacer;

    buf->base = (char *)malloc(buf->len);
    memset(buf->base, 0, buf->len);
    memcpy(buf->base, responseMessage.c_str(), buf->len - spacer);

    req = (uv_udp_send_t *)malloc(sizeof(uv_udp_send_t));

    req->data = (void *)this;

//    struct sockaddr_in udp_send_addr;
//    uv_ip4_addr("192.168.0.4", 5061, &udp_send_addr);
    //udp_send_addr.sin_port = 5060;
    //status = uv_udp_bind(&udpClientSocket, (const struct sockaddr *)&udp_send_addr, 0);
//    if (status) {
//        fprintf(stderr, "UDP bind returned %d, %s, %s\n", status, uv_err_name(status), uv_strerror(status));
//    }
//    udp_send_addr.sin_port = 5060;




    //fprintf(stderr, "Sending to %s:%d UDP port %d\n", remoteSipAddress, remoteSipPort, destinationAddress.sin_port);
    status = uv_udp_send(req, &udpServerSocket, buf, 1, (const sockaddr *) &destinationAddress, &onUdpSend);

    if (status) {
        fprintf(stderr, "SipInterface::sendMessage send returned %d, %s\n", status, uv_err_name(status));
    }

    return status;
}



