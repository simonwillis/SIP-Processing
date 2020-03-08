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

    logger = spdlog::get("stdlogger");

    handler = eventHandler;

    // We can use a single parser object here as we received UDP messages in their totality
    // SIP Messages on TCP can have long running connections that handle blocks of data.
    // in the case of TCP we set up a sip parser per TCP Connection.
    parser = new SipParser(sipUdpMessageHandler, this);
}

SipInterface::~SipInterface() {

    //TODO Implement SipInterface DTOR
}

void SipInterface::processMessage(SipMessage * sipMessage) {

    SipTransaction *transaction = nullptr;

    if (!sipMessage) {
        logger->warn("SipInterface cannot process empty message");
        return;
    }

    transaction = dialogueManager.updateTransaction(sipMessage);

    if (!transaction) {
        logger->warn("SipInterface::processMessage: updateTransaction returned null");
        return;
    }
    uint32_t channelId = transaction->channelId;


    if (sipMessage->isRequest()) {
        SipRequest *sipRequest = (SipRequest *) sipMessage;

        logger->debug("SIP RX <<<=== [channelId={}] [method={}] {}", channelId,
                SipMessage::getTypeStr(sipRequest->getRequestType()), sipRequest->message);

        SipMessage::SipMessageType requestType = ((SipRequest *) sipMessage)->getRequestType();
        switch (requestType) {
            case SipMessage::SipMessageType::INVITE:
                processInvite((SipInviteRequest *) sipMessage);
                break;
            case SipMessage::SipMessageType::ACK:
                processAck((SipAckRequest *) sipMessage);
                break;
            case SipMessage::SipMessageType::BYE:
                processBye((SipByeRequest *) sipMessage);
                break;
            case SipMessage::SipMessageType::CANCEL:
                processCancel((SipCancelRequest *) sipMessage);
                break;
            case SipMessage::SipMessageType::OPTIONS:
                processOptions((SipOptionsRequest *) sipMessage);
                break;
            case SipMessage::SipMessageType::SUBSCRIBE:
                processSubscribe((SipSubscribeRequest *) sipMessage);
                break;
            default:
                logger->warn("Not currently handling {} Request in inbound SIP stream handler [channel={}]",
                        sipMessage->getSubTypeDescriptionString(), channelId);
                break;
        }
    } else {
        SipMessage::SipMessageType responseType = ((SipResponse *) sipMessage)->getResponseType();
        switch (responseType) {
            default:
                logger->warn( "Not currently handling {} Response in inbound SIP stream handler [channel={}]",
                        sipMessage->getSubTypeDescriptionString(), channelId);
                break;
        }
    }
}


void  SipInterface::sipUdpMessageHandler(SipMessage * sipMessage, void * callbackData) {
    SipInterface * that = (SipInterface *)callbackData;
    that->logger->debug("SIP (UDP) Event Handler triggered with [{} {}] [UDP]",
            sipMessage->getSubTypeDescriptionString(), sipMessage->getTypeDescriptionString());

    that->processMessage(sipMessage);
    return;
}

void  SipInterface::sipTcpMessageHandler(SipMessage * sipMessage, void * callbackData) {
    SipConnection * sipConnection = (SipConnection *) callbackData;
    SipInterface * that = (SipInterface *) sipConnection->data;

    that->logger->debug("SIP (TCP) Event Handler triggered with sipMessage [{} {}] [hasBody={}] [TCP] [connId={}]",
            sipMessage->getSubTypeDescriptionString(), sipMessage->getTypeDescriptionString(),
            sipMessage->hasBody() ? "true":"false",
            (sipConnection ? sipConnection->connectionId : 0));

    if (sipMessage->hasBody(SipMessageBody::BodyType::SDP)) {
        that->logger->debug("SipInterface::sipTcpMessageHandler: message has an SDP body");
    }



    that->processMessage(sipMessage);
    return;
}

void SipInterface::onUdpSend(uv_udp_send_t *req, int status) {

    if (status) {

    }

}

void SipInterface::onUdpRecv(uv_udp_t *handle, long int bytes_read, const struct uv_buf_t *uv_buffer,
                             const struct sockaddr *addr, unsigned int flags) {

    // the handle's data field holds the reference to the instance of the interface (set on call to Start())
    SipInterface * that = (SipInterface *)handle->data;

    if (bytes_read < 0) {

        that->logger->warn("SIP UDP Stream Handler. Read error [errorCode={}] [errorMessage={}]", bytes_read, uv_err_name(bytes_read));

    } else if (bytes_read > 0) {

        char * p;
        long int i;
        for (i = 0, p = uv_buffer->base; i < bytes_read; ++i, p++) {
            printChar(*p);
        }

        size_t count;
        char buf[4096];
        MessageBuffer messageBuffer(4096);

        messageBuffer.push(uv_buffer->base, (size_t)bytes_read);

        // UDP SIP Messages do not necessarily terminate with a double CRLF, typically just a single
        do {
            count = messageBuffer.pull(buf, sizeof(buf), SipParser::messageDelimiter);
            if (count == 0 && messageBuffer.getStoredByteCount()) {
                count = messageBuffer.pull(buf, sizeof(buf), messageBuffer.getStoredByteCount());
            }

            that->parser->loadMessageData(buf, count);
        } while (messageBuffer.getStoredByteCount());

    }

    if (uv_buffer->len) {
        free(uv_buffer->base);
    }
}

void SipInterface::onTcpRecv(uv_stream_t *client, ssize_t bytes_read, const uv_buf_t *uv_buffer) {

    SipConnection * sipConnection = (SipConnection *) client->data;
    SipInterface * that = (SipInterface *)sipConnection->data;

    if (bytes_read == UV_EOF) {
        // XXX: this is actually a bug, since client writes could back up (see ./test-client.sh) and therefore
        // some chunks to be written be left in the pipe.
        // Closing in that case results in: [ECANCELED: operation canceled]
        // Not closing here avoids that error and a few more chunks get written.
        // But even then not ALL of them make it through and things get stuck.
        uv_close((uv_handle_t*) client, NULL);

        that->logger->debug("SipInterface::onTcpRecv: closed client connection");

    } else if (bytes_read > 0) {

        if (that == nullptr) {
            spdlog::get("stdlogger")->error("SipInterface::onTcpRecv: context data not set");
            return;
        }

        that->logger->debug("SipInterface::onTcpRecv: pushing {} bytes onto message buffer", bytes_read);
        sipConnection->messageBuffer->push(uv_buffer->base, (size_t)bytes_read);

        // The message buffer drains out on delimiters, SIP Messages are always delimited with "\r\n\r\n"
        // The SipParser spits out messages when it has enough data.
        char buf[1024];
        size_t count;

        do {
            if (sipConnection->pendingMessageBytes) {
                that->logger->debug("SipInterface::onTcpRecv: sipConnection pending {} bytes", sipConnection->pendingMessageBytes);
                count = sipConnection->messageBuffer->pull(buf, sizeof(buf), sipConnection->pendingMessageBytes);
            } else {
                count = sipConnection->messageBuffer->pull(buf, sizeof(buf), SipParser::messageDelimiter);
            }
            that->logger->debug("SipInterface::onTcpRecv: Pulled {} bytes from buffer", count);
            if (count) {
                sipConnection->pendingMessageBytes = sipConnection->sipParser->loadMessageData(buf, count);
                that->logger->debug("SipInterface::onTcpRecv: Load Message returned pending byte count {}", sipConnection->pendingMessageBytes);
            }
        } while (count > 0);

        that->logger->debug("SipInterface::onTcpRecv: finished pulling from buffer");
    }
    if (bytes_read == 0) free(uv_buffer->base);
}

void SipInterface::onTcpConnection(uv_stream_t *server, int status) {

    if (status) {
        spdlog::get("stdlogger")->error("onTcpConnection error {}, {}", status, uv_err_name(status));
        return;
    }

    SipInterface * that = (SipInterface *) server->data;

    uint32_t connectionId = that->getNextConnectionId();

    SipConnection * sipConnection = new SipConnection(connectionId, that, server, sipTcpMessageHandler);

    that->logger->info("Allocated new connectionId {}", connectionId);

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(uv_default_loop(), client);

    client->data = (void *) sipConnection;

    int result = uv_accept(server, (uv_stream_t*) client);
    if (result) {
        that->logger->warn("Failed to accept connection error={}, {}\n", result, uv_err_name(result));
        uv_close((uv_handle_t*) client, NULL);
    } else {
        uv_read_start((uv_stream_t *) client, alloc_buffer, onTcpRecv);
    }
}

void SipInterface::alloc_buffer(struct uv_handle_s* handle, long unsigned int size, struct uv_buf_t* buf) {

    long unsigned int bufSize = 8192;

    buf->base = (char *)malloc(bufSize);
    buf->len = bufSize;
}

int SipInterface::Start(int port) {

    int result, status;

    this->logger->info("Starting SIP on port {}", port);

    uv_udp_init(uv_default_loop(), &udpServerSocket);
    udpServerSocket.data = (void *) this;
    struct sockaddr_in udp_recv_addr;
    uv_ip4_addr("0.0.0.0", port, &udp_recv_addr);
    status = uv_udp_bind(&udpServerSocket, (const struct sockaddr *)&udp_recv_addr, UV_UDP_REUSEADDR);
    if (status) {
        this->logger->error("UDP bind server on port {} returned {}, {}, {}", port, status, uv_err_name(status), uv_strerror(status));
    }
    uv_udp_recv_start(&udpServerSocket, alloc_buffer, onUdpRecv);

    uv_tcp_init(uv_default_loop(), &tcpServer);
    struct sockaddr_in tcp_recv_addr;
    result = uv_ip4_addr("0.0.0.0", port, &tcp_recv_addr);

    if (result) {
        this->logger->error("Failed to set up tcp server address in start error = {} ({})", result, uv_err_name(result));
    } else {
        uv_tcp_bind(&tcpServer, (struct sockaddr *) &tcp_recv_addr, 0);
        tcpServer.data = (void *) this;

        this->logger->info("Starting listen on TCP port {}", port);
        result = uv_listen((uv_stream_t *) &tcpServer, 64, onTcpConnection);
        if (result) {
            this->logger->error("failed to listen on port {}, error {} ({})", port, result, uv_err_name(result));
        }
    }

//    uv_udp_init(uv_default_loop(), &udpClientSocket);
//    struct sockaddr_in udp_send_addr;
//    uv_ip4_addr("192.168.0.4", 5061, &udp_send_addr);
//    //udp_send_addr.sin_port = 5061;
//    status = uv_udp_bind(&udpClientSocket, (const struct sockaddr *)&udp_send_addr, UV_UDP_REUSEADDR);
//    if (status) {
//        this->logger->error("UDP bind client on port {} returned {}, {}, {}\n", port, status, uv_err_name(status), uv_strerror(status));
//    }

//    uv_udp_init(uv_default_loop(), &udpClientSocket);
//    uv_ip4_addr("0.0.0.0", port, &udp_send_addr);

    return 0;
}


void SipInterface::tcpOnWrite(uv_write_t *req, int status) {

    uv_req_t* wr = (uv_req_t*) req;

    free(wr);
}

int SipInterface::Stop() {

    //TODO Implement SipInterface::Stop()
    this->logger->warn("SipInterface::Stop Not Implemented Yet");
    return 0;
}

std::string SipInterface::getMediaOffer(uint32_t channelId) {

    this->logger->debug("SipInterface::getMediaOffer [channelId={}]", channelId);
    SipDialogue * dialogue = dialogueManager.retrieveDialogue(channelId);
    if (dialogue) {
        SipTransaction * transaction = dialogue->getCurrentTransaction();
        if (transaction) {
            this->logger->debug("SipInterface::getMediaOffer [channelId={}] [offer={}]", channelId, transaction->sdpOffer.c_str());
            return transaction->sdpOffer;
        } else {
            this->logger->warn("SipInterface::getMediaOffer [channelId={}] No transaction found", channelId);
        }
    } else {
        this->logger->warn("SipInterface::getMediaOffer [channelId={}] No dialogue found", channelId);
    }
    // if we get here then there is no offer data to return,
    return std::string();
}

int SipInterface::answerCall(uint32_t channelId, const char * tag, const char * contactField, std::string sdpAnswer) {


    this->logger->debug("SipInterface::answerCall TBC [channelId={}] [tag={}] [contact={}]", channelId, tag, contactField);
    SipDialogue * dialogue = dialogueManager.retrieveDialogue(channelId);
    if (! dialogue) {
        this->logger->warn("SipInterface::answerCall failed to locate dialogue for channel [channelId={}]", channelId);
        return 0;
    }

    this->logger->debug("Answering call via %s (TBC)", SipMessage::getTransportTypeStr(dialogue->getTransportType()));

    SipTransaction * transaction = dialogue->getCurrentTransaction();

    if (! transaction ) {
        this->logger->warn("SipInterface::answerCall no active transaction on channel {}", channelId);
    } else {
        this->logger->debug("Answering call. Have a transaction on channel {}", transaction->channelId);
        if (transaction->getType() != SipMessage::SipMessageType::INVITE) {
            this->logger->warn("SipInterface::answerCall cannot complete on channel {} should be 'INVITE' have {}", channelId, transaction->toString().c_str());
        }
        if (! transaction->sipRequest) {
            this->logger->warn( "Transaction on channel {} does not have a sipRequest defined", channelId);
            return 1;
        }

        SipInviteRequest * sipInviteRequest = (SipInviteRequest *) transaction->sipRequest;

        this->logger->debug("Building response string with which to answer call on channel {}", channelId);
        string responseString = sipInviteRequest->buildAnswerResponse(dialogue->getResponseTag(), sdpAnswer);

        this->logger->debug("SipInterface::answerCall on channel {} prepared response message as {}", channelId, responseString.c_str());
        sendMessage(transaction->sipRequest->getRemoteAddress().c_str(), transaction->sipRequest->getRemotePort(), transaction->sipRequest->getTransportType(), responseString);
    }

    return 0;
}

int SipInterface::startRecordingCall(uint32_t channelId, const char * filename) {

    this->logger->debug("SipInterface::startRecordingCall TBC on channel {}", channelId);

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

    //TODO Implement SipInterface::stopRecordingCall

    this->logger->debug("SipInterface::stopRecordingCall TBC on channel {}", channelId);
    return 0;
}

int SipInterface::rejectCall(uint32_t channelId, int reasonCode, const char * reasonMessage) {

    //TODO Implement SipInterface::rejectCall

    this->logger->debug("SipInterface::rejectCall TBC on channel {}", channelId);

    return 0;
}

int SipInterface::clearCall(uint32_t channelId, int reasonCode, const char * reasonMessage) {

    //TODO Implement SipInterface::clearCall

    this->logger->debug("SipInterface::clearCall TBC on channel {}", channelId);

    return 0;
}

void SipInterface::processSubscribe(SipSubscribeRequest * request) {
    sendMessage(request->getRemoteAddress().c_str(),
                request->getRemotePort(),
                request->getTransportType(),
                request->buildSubscriptionAcceptedResponse("subscription"));
}

void SipInterface::processInvite(SipInviteRequest * request) {

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());

    if (! dialogue) {
        this->logger->warn("SipInterface::processInvite Session not set against request, aborting");
        return;
    } else {
        this->logger->debug("SipInterface: processInvite on channel {}, callId {}", dialogue->getChannelId(), dialogue->getCallId().c_str());
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

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());
    if (! dialogue) {
        this->logger->warn("SipInterface::processAck Session not set against request, aborting");
        return;
    }

    if (dialogue->getState() == SipDialogue::DialogueState::INCOMING) {
        dialogue->setState(SipDialogue::DialogueState::CONNECTED);
        CallAnsweredEvent event = CallAnsweredEvent(dialogue->getChannelId(), request->getCallId());
        handler(&event);
    }
}

void SipInterface::processBye(SipByeRequest * request) {

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());
    if (! dialogue) {
        this->logger->warn("SipInterface::processBye Session not set against request on channel {}, aborting", request->getChannelId());
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

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());
    if (! dialogue) {
        logger->error("SipInterface::processCancel Session not set against request, aborting");
        return;
    }

    if (dialogue->getState() == SipDialogue::DialogueState::INCOMING) {
        CallClearedEvent event = CallClearedEvent(dialogue->getChannelId(), request->getCallId(), 487);
        handler(&event);
    }
}

void SipInterface::processOptions(SipOptionsRequest * request) {

    SipDialogue * dialogue = dialogueManager.retrieveDialogue(request->getCallId());
    if (! dialogue) {
        logger->error("SipInterface::processOptions Session not set against request, aborting");
        return;
    }

    //TODO Fill in functionality for basic OPTIONS response
    //TODO Set up and send an options event to the controller

}

void SipInterface::processResponse(SipResponse * response) {

    //TODO Implement SipInterface::processResponse
    logger->warn("SipInterface::processResponse Not Implemented Yet");
    return;
}

SipTransaction SipInterface::getTransaction(uint32_t channelId) {

    //TODO Implement SipInterface::getTransaction
    logger->warn("SipInterface::getTransaction Not Implemented Yet");

    return SipTransaction();
}

int SipInterface::sendMessage (
                    const char *remoteSipAddress,
                    int remoteSipPort,
                    SipMessage::SipMessageTransportType transportType,
                    std::string responseMessage) {

    int status;
    uv_buf_t *buf;
    uv_udp_send_t *req;
    struct sockaddr_in destinationAddress;
    int spacer = 0;

    int destPort = remoteSipPort;

    logger->info("SIP TX ===>>>\n{}:{}:{}\n{}",SipMessage::getTransportTypeStr(transportType), remoteSipAddress, remoteSipPort, responseMessage.c_str());

    uv_ip4_addr(remoteSipAddress, destPort, &destinationAddress);

    buf = (uv_buf_t *)malloc(sizeof(uv_buf_t));
    buf->len = responseMessage.size() + spacer;

    buf->base = (char *)malloc(buf->len);
    memset(buf->base, 0, buf->len);
    memcpy(buf->base, responseMessage.c_str(), buf->len - spacer);

    req = (uv_udp_send_t *)malloc(sizeof(uv_udp_send_t));

    req->data = (void *)this;

    status = uv_udp_send(req, &udpServerSocket, buf, 1, (const sockaddr *) &destinationAddress, &onUdpSend);

    if (status) {
        logger->error("SipInterface::sendMessage send returned {}, {}", status, uv_err_name(status));
    }

    return status;
}



