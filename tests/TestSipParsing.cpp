//
// Created by Simon Willis on 23/05/2016.
//

//#include <cstdio>
//#include <string>
#include "../src/components/sip/messages/SipMessage.h"
#include "../src/components/sip/SipParser.h"
#include "../src/components/utilities/MessageBuffer.h"

#include "data/SipMessageSamples.hpp"


void printParsedMessage(SipMessage & message) {

}

void eventHandler(SipMessage * sipMessage, void * callbackData) {
    fprintf(stderr, "Event Handler triggered with SIP %s %s\n",
            sipMessage->getSubTypeDescriptionString(), sipMessage->getTypeDescriptionString());

}


int main(int argc, char * argv[]) {

    SipParser parser(eventHandler);
    SipMessage * sipMessage;

    std::vector<char> sipDelimiter = {'\r', '\n', '\r', '\n'};

    MessageBuffer mb = MessageBuffer(16192);

//    char buf[16192];

    parser.loadMessageData(ciscoSiprecInvite.c_str(), ciscoSiprecInvite.size());
    parser.loadMessageData(standarOkToByeResponse.c_str(), standarOkToByeResponse.size());
    parser.loadMessageData(standardTryingResponse.c_str(), standardTryingResponse.size());



//    sipMessage = parser.(ciscoSiprecInvite.c_str(), ciscoSiprecInvite.size());
//
//    sipMessage = parser.parse(inviteWithUnusualBody.c_str(), inviteWithUnusualBody.size());
//
//    sipMessage = parser.parse(standardInviteWithSdp.c_str(), standardInviteWithSdp.size());
//
//    sipMessage = parser.parse(standardAckMessage.c_str(), standardAckMessage.size());
//
//    sipMessage = parser.parse(standardTryingResponse.c_str(), standardTryingResponse.size());
//
//    sipMessage = parser.parse(standardByeMessage.c_str(), standardByeMessage.size());
//
//    sipMessage = parser.parse(standarOkToByeResponse.c_str(), standarOkToByeResponse.size());




//    fprintf(stderr, "Tests Main, input is %ld bytes\n", sampleSipInvite.length());
//    SipRequest * sipMessage = SipRequest::build_sip_request(sampleSipInvite.c_str(), sampleSipInvite.length());
//    fprintf(stderr, "Have sip event type %s\n", sipMessage->getRequestMethod());
//
//    SdpReceivedOffer answer = SdpReceivedOffer(sampleSdpOffer.c_str(), sampleSdpOffer.length());
//    fprintf(stderr, "SDP Offer\n%s\n", answer.getOfferContent());
//    fprintf(stderr, "SDP Response\n%s\n", answer.getResponseContent());

    return 0;
}


