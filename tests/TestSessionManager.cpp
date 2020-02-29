//
// Created by Simon Willis on 03/06/2016.
//


#include "../src/components/sip/SipDialogueManager.h"
#include "../src/components/utilities/MessageBuffer.h"

#include "data/SipMessageSamples.hpp"

using namespace std;




//TODO Extend out SessionManager Tests

SipDialogueManager sm;


void SipMessageHandler(SipMessage * sipMessage, void * cbData) {
    fprintf(stderr, "SipMessage handler called\n");
    sm.updateTransaction(sipMessage);

}

SipParser sipParser = SipParser(SipMessageHandler, (void *) 1);


int main(int argc, char * argv[]) {
    string callId1 = "call-id-1";
    string callId2 = "call-id-2";
    uint32_t channelId = 0;
    int error = 0;
    char sipData[4096];
    std::vector<char> sipMessageDelimiter = {'\r','\n','\r','\n'};
    size_t byteCount;

    //fprintf(stderr, "UPDATE SIP PARSER CALL\n");

    MessageBuffer messageBuffer = MessageBuffer(8192);

    messageBuffer.push(ciscoSiprecInvite.c_str(), ciscoSiprecInvite.size());

    while ((byteCount = messageBuffer.pull(sipData, sizeof(sipData), sipMessageDelimiter))) {
        sipParser.loadMessageData(sipData, byteCount);
    }


    getchar();

    return 0;
}
