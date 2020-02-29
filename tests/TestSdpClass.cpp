//
// Created by simon on 07/11/16.
//


#include "../src/components/sdp/Sdp.h"

const char * sample1 =
"v=0\r\n"
"o=alice 2890844526 2890844526 IN IP4 host.atlanta.example.com\r\n"
"s=-\r\n"
"c=IN IP4 host.atlanta.example.com\r\n"
"t=0 0\r\n"
"a=general:1 param data 1\r\n"
"a=general:2 param data 2\r\n"
"m=audio 49170 RTP/AVP 0 8 97\r\n"
"a=sendonly\r\n"
"a=rtpmap:0 PCMU/8000\r\n"
"a=rtpmap:8 PCMA/8000\r\n"
"a=rtpmap:97 iLBC/8000\r\n"
"m=video 51372 RTP/AVP 31 32\r\n"
"a=rtpmap:31 H261/90000\r\n"
"a=rtpmap:32 MPV/90000\r\n";

const char * sample2 =
"v=0\r\n"
"o=- 1499700059441093 1 IN IP4 10.141.105.155\r\n"
"s=X-Lite release 4.9.8 stamp 84254\r\n"
"c=IN IP4 10.141.105.155\r\n"
"t=0 0\r\n"
"m=audio 55558 RTP/AVP 0\r\n"
"a=sendrecv";

const char * sample3 =
"v=0\r\n"
"o=BroadWorks 3461905 3 IN IP4 199.19.193.10\r\n"
"s=-\r\n"
"c=IN IP4 199.19.193.23\r\n"
"t=0 0\r\n"
"m=audio 10528 RTP/AVP 0 9 8 97\r\n"
"a=rtpmap:0 PCMU/8000\r\n"
"a=rtpmap:9 G722/8000\r\n"
"a=rtpmap:8 PCMA/8000\r\n"
"a=rtpmap:97 AMR-WB/16000\r\n"
"a=fmtp:97 max-red=0; mode-change-capability=2\r\n"
"a=ptime:20\r\n"
"a=maxptime:20\r\n"
"a=sendonly\r\n"
"a=label:1\r\n"
"m=audio 10536 RTP/AVP 0 9 8 97\r\n"
"a=rtpmap:0 PCMU/8000\r\n"
"a=rtpmap:9 G722/8000\r\n"
"a=rtpmap:8 PCMA/8000\r\n"
"a=rtpmap:97 AMR-WB/16000\r\n"
"a=fmtp:97 max-red=0; mode-change-capability=2\r\n"
"a=ptime:20\r\n"
"a=maxptime:20\r\n"
"a=sendonly\r\n"
"a=label:2\r\n";



void loadFromString(const char * s, Sdp &sdp) {
    sdp = Sdp(s, strlen(s));
}

void writeOut(Sdp &sdp) {
    fprintf(stderr, "===DATA===\n'%s'\n", sdp.toString().c_str());
    fprintf(stderr, "===JSON===\n%s\n", sdp.toJson().c_str());
}



int main(int argc, char * argv[]) {

    Sdp sdp;

    loadFromString(sample1, sdp);
    writeOut(sdp);

    if (sdp.toString().compare(std::string(sample1))) {
        fprintf(stderr, "Sample1 Output does not match input\n");
        fprintf(stderr, "ORIGINAL\n'%s'\n", sample1);
        fprintf(stderr, "REBUILT\n'%s'\n", sdp.toString().c_str());
    } else {
        fprintf(stderr, "Sample1 Output matches input\n");
    }

    loadFromString(sample2, sdp);
    writeOut(sdp);
    if (sdp.toString().compare(sample2)) {
        fprintf(stderr, "Sample2 Output does not match input\n");
        fprintf(stderr, "%s\n", sample2);
    } else {
        fprintf(stderr, "Sample2 Output matches input\n");
    }

    loadFromString(sample3, sdp);
    writeOut(sdp);
    if (sdp.toString().compare(sample3)) {
        fprintf(stderr, "Sample3 Output does not match input\n");
        fprintf(stderr, "%s\n", sample3);
    } else {
        fprintf(stderr, "Sample3 Output matches input\n");
    }

}