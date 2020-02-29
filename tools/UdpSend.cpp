//
// Created by Simon Willis on 16/06/2016.
//

#include <uv.h>
#include <string>
#include <cstdlib>
#include <cstring> // required for gcc compile on linux for memcpy - ignore the unused import notification

using namespace std;

std::string simpleSipInvite = std::string (
"INVITE sip:service@192.168.33.10:5060 SIP/2.0\r\n" \
"Via: SIP/2.0/UDP 192.168.33.1:5060;branch=z9hG4bK-19644-1-0\r\n" \
"From: sipp <sip:sipp@192.168.33.1:5060>;tag=19644SIPpTag001\r\n" \
"To: service <sip:service@192.168.33.10:5060>\r\n" \
"Call-ID: 1-19644@192.168.33.1\r\n" \
"CSeq: 1 INVITE\r\n" \
"Contact: sip:sipp@192.168.33.1:5060\r\n" \
"Max-Forwards: 70\r\n" \
"Subject: Performance Test\r\n" \
"Content-Type: application/sdp\r\n" \
"Content-Length:   135\r\n" \
"\r\n" \
"v=0\r\n" \
"o=user1 53655765 2353687637 IN IP4 192.168.33.1\r\n" \
"s=-\r\n" \
"c=IN IP4 192.168.33.1\r\n" \
"t=0 0\r\n" \
"m=audio 6000 RTP/AVP 0\r\n" \
"a=rtpmap:0 PCMU/8000\r\n" \
"\r\n"
);


// "Require: siprec\r\n" \


std::string ciscoSiprecInvite = std::string (
"INVITE sip:9999@192.168.33.10:5060 SIP/2.0\r\n" \
"Via: SIP/2.0/UDP 192.168.33.1:5060;branch=z9hG4bK12E46\r\n" \
"From: <sip:192.168.33.1>;tag=244C3D0-301\r\n" \
"To: <sip:9999@192.168.33.10>\r\n" \
"Date: Fri, 06 May 2016 07:57:48 GMT\r\n" \
"Call-ID: F2C554A-129711E6-803DF172-2528FD61@192.168.33.1\r\n" \
"Supported: 100rel,timer,resource-priority,replaces,sdp-anat\r\n" \
"Min-SE:  1800\r\n" \
"Cisco-Guid: 0202204008-0311890406-2150560114-0623443297\r\n" \
"User-Agent: Cisco-SIPGateway/IOS-15.6.1.T0a\r\n" \
"Allow: INVITE, OPTIONS, BYE, CANCEL, ACK, PRACK, UPDATE, REFER, SUBSCRIBE, NOTIFY, INFO, REGISTER\r\n" \
"CSeq: 101 INVITE\r\n" \
"Max-Forwards: 70\r\n" \
"Timestamp: 1462521468\r\n" \
"Contact: <sip192.168.33.1:5060>;+sip.src\r\n" \
"Expires: 180\r\n" \
"Allow-Events: telephone-event\r\n" \
"Content-Type: multipart/mixed;boundary=uniqueBoundary\r\n" \
"Mime-Version: 1.0\r\n" \
"Content-Length: 2249\r\n" \
"\r\n" \
"--uniqueBoundary \r\n" \
"Content-Type: application/sdp \r\n" \
"Content-Disposition: session;handling=required\r\n" \
"\r\n" \
"v=0\r\n" \
"o=CiscoSystemsSIP-GW-UserAgent 1478 8649 IN IP4 192.168.33.1\r\n" \
"s=SIP Call\r\n" \
"c=IN IP4 192.168.33.1\r\n" \
"t=0 0\r\n" \
"m=audio 16412 RTP/AVP 8 19\r\n" \
"c=IN IP4 192.168.33.1\r\n" \
"a=rtpmap:8 PCMA/8000\r\n" \
"a=rtpmap:19 CN/8000\r\n" \
"a=ptime:20\r\n" \
"a=sendonly\r\n" \
"a=label:1\r\n" \
"m=audio 16414 RTP/AVP 8 19\r\n" \
"c=IN IP4 192.168.33.1\r\n" \
"a=rtpmap:8 PCMA/8000\r\n" \
"a=rtpmap:19 CN/8000\r\n" \
"a=ptime:20\r\n" \
"a=sendonly\r\n" \
"a=label:2\r\n" \
"\r\n" \
"--uniqueBoundary\r\n" \
"Content-Type: application/rs-metadata+xml\r\n" \
"Content-Disposition: recording-session\r\n" \
"\r\n" \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?> \r\n" \
" <recording xmlns=\"urn:ietf:params:xml:ns:recording:1\"> \r\n" \
"   <datamode>complete</datamode> \r\n" \
"   <session session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <start-time>2016-05-06T07:57:48.526Z</start-time> \r\n" \
"   </session> \r\n" \
"   <participant participant_id=\"Dyu5IhKXEeaANvFyJSj9YQ==\"> \r\n" \
"       <nameID aor=\"sip:448707803128@192.168.33.1\"> \r\n" \
"       </nameID> \r\n" \
"   </participant> \r\n" \
"   <participantsessionassoc participant_id=\"Dyu5IhKXEeaANvFyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <associate-time>2016-05-06T07:57:48.526Z</associate-time> \r\n" \
"   </participantsessionassoc> \r\n" \
"   <stream stream_id=\"DyxVShKXEeaAO/FyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <label>1</label> \r\n" \
"   </stream> \r\n" \
"   <participant participant_id=\"Dyu5IhKXEeaAN/FyJSj9YQ==\"> \r\n" \
"       <nameID aor=\"sip:8001@172.17.12.99\"> \r\n" \
"       </nameID> \r\n" \
"   </participant> \r\n" \
"   <participantsessionassoc participant_id=\"Dyu5IhKXEeaAN/FyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <associate-time>2016-05-06T07:57:48.526Z</associate-time> \r\n" \
"   </participantsessionassoc> \r\n" \
"   <stream stream_id=\"DyxVShKXEeaAPPFyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <label>2</label> \r\n" \
"   </stream> \r\n" \
"   <participantstreamassoc participant_id=\"Dyu5IhKXEeaANvFyJSj9YQ==\"> \r\n" \
"       <send>DyxVShKXEeaAO/FyJSj9YQ==</send> \r\n" \
"       <recv>DyxVShKXEeaAPPFyJSj9YQ==</recv> \r\n" \
"   </participantstreamassoc> \r\n" \
"   <participantstreamassoc participant_id=\"DyxVShKXEeaAO/FyJSj9YQ==\"> \r\n" \
"       <send>DyxVShKXEeaAPPFyJSj9YQ==</send> \r\n" \
"       <recv>DyxVShKXEeaAO/FyJSj9YQ==</recv> \r\n" \
"   </participantstreamassoc> \r\n" \
"</recording> \r\n" \
"\r\n" \
"--uniqueBoundary-- \r\n" \
"\r\n");

std::string ciscoSiprecBody = std::string(
"--uniqueBoundary \r\n" \
"Content-Type: application/sdp \r\n" \
"Content-Disposition: session;handling=required\r\n" \
"\r\n" \
"v=0\r\n" \
"o=CiscoSystemsSIP-GW-UserAgent 1478 8649 IN IP4 192.168.33.1\r\n" \
"s=SIP Call\r\n" \
"c=IN IP4 192.168.33.1\r\n" \
"t=0 0\r\n" \
"m=audio 16412 RTP/AVP 8 19\r\n" \
"c=IN IP4 192.168.33.1\r\n" \
"a=rtpmap:8 PCMA/8000\r\n" \
"a=rtpmap:19 CN/8000\r\n" \
"a=ptime:20\r\n" \
"a=sendonly\r\n" \
"a=label:1\r\n" \
"m=audio 16414 RTP/AVP 8 19\r\n" \
"c=IN IP4 192.168.33.1\r\n" \
"a=rtpmap:8 PCMA/8000\r\n" \
"a=rtpmap:19 CN/8000\r\n" \
"a=ptime:20\r\n" \
"a=sendonly\r\n" \
"a=label:2\r\n" \
"\r\n" \
"--uniqueBoundary\r\n" \
"Content-Type: application/rs-metadata+xml\r\n" \
"Content-Disposition: recording-session\r\n" \
"\r\n" \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?> \r\n" \
" <recording xmlns=\"urn:ietf:params:xml:ns:recording:1\"> \r\n" \
"   <datamode>complete</datamode> \r\n" \
"   <session session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <start-time>2016-05-06T07:57:48.526Z</start-time> \r\n" \
"   </session> \r\n" \
"   <participant participant_id=\"Dyu5IhKXEeaANvFyJSj9YQ==\"> \r\n" \
"       <nameID aor=\"sip:448707803128@192.168.33.1\"> \r\n" \
"       </nameID> \r\n" \
"   </participant> \r\n" \
"   <participantsessionassoc participant_id=\"Dyu5IhKXEeaANvFyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <associate-time>2016-05-06T07:57:48.526Z</associate-time> \r\n" \
"   </participantsessionassoc> \r\n" \
"   <stream stream_id=\"DyxVShKXEeaAO/FyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <label>1</label> \r\n" \
"   </stream> \r\n" \
"   <participant participant_id=\"Dyu5IhKXEeaAN/FyJSj9YQ==\"> \r\n" \
"       <nameID aor=\"sip:8001@172.17.12.99\"> \r\n" \
"       </nameID> \r\n" \
"   </participant> \r\n" \
"   <participantsessionassoc participant_id=\"Dyu5IhKXEeaAN/FyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <associate-time>2016-05-06T07:57:48.526Z</associate-time> \r\n" \
"   </participantsessionassoc> \r\n" \
"   <stream stream_id=\"DyxVShKXEeaAPPFyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <label>2</label> \r\n" \
"   </stream> \r\n" \
"   <participantstreamassoc participant_id=\"Dyu5IhKXEeaANvFyJSj9YQ==\"> \r\n" \
"       <send>DyxVShKXEeaAO/FyJSj9YQ==</send> \r\n" \
"       <recv>DyxVShKXEeaAPPFyJSj9YQ==</recv> \r\n" \
"   </participantstreamassoc> \r\n" \
"   <participantstreamassoc participant_id=\"DyxVShKXEeaAO/FyJSj9YQ==\"> \r\n" \
"       <send>DyxVShKXEeaAPPFyJSj9YQ==</send> \r\n" \
"       <recv>DyxVShKXEeaAO/FyJSj9YQ==</recv> \r\n" \
"   </participantstreamassoc> \r\n" \
"</recording> \r\n" \
"\r\n" \
"--uniqueBoundary-- \r\n" \
"\r\n"
);

std::string ciscoXmlBody = string(
"<?xml version=\"1.0\" encoding=\"UTF-8\"?> \r\n" \
" <recording xmlns=\"urn:ietf:params:xml:ns:recording:1\"> \r\n" \
"   <datamode>complete</datamode> \r\n" \
"   <session session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <start-time>2016-05-06T07:57:48.526Z</start-time> \r\n" \
"   </session> \r\n" \
"   <participant participant_id=\"Dyu5IhKXEeaANvFyJSj9YQ==\"> \r\n" \
"       <nameID aor=\"sip:448707803128@192.168.33.1\"> \r\n" \
"       </nameID> \r\n" \
"   </participant> \r\n" \
"   <participantsessionassoc participant_id=\"Dyu5IhKXEeaANvFyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <associate-time>2016-05-06T07:57:48.526Z</associate-time> \r\n" \
"   </participantsessionassoc> \r\n" \
"   <stream stream_id=\"DyxVShKXEeaAO/FyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <label>3</label> \r\n" \
"   </stream> \r\n" \
"   <participant participant_id=\"Dyu5IhKXEeaAN/FyJSj9YQ==\"> \r\n" \
"       <nameID aor=\"sip:8001@172.17.12.99\"> \r\n" \
"       </nameID> \r\n" \
"   </participant> \r\n" \
"   <participantsessionassoc participant_id=\"Dyu5IhKXEeaAN/FyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <associate-time>2016-05-06T07:57:48.526Z</associate-time> \r\n" \
"   </participantsessionassoc> \r\n" \
"   <stream stream_id=\"DyxVShKXEeaAPPFyJSj9YQ==\" session_id=\"Dyu5IhKXEeaANfFyJSj9YQ==\"> \r\n" \
"       <label>4</label> \r\n" \
"   </stream> \r\n" \
"   <participantstreamassoc participant_id=\"Dyu5IhKXEeaANvFyJSj9YQ==\"> \r\n" \
"       <send>DyxVShKXEeaAO/FyJSj9YQ==</send> \r\n" \
"       <recv>DyxVShKXEeaAPPFyJSj9YQ==</recv> \r\n" \
"   </participantstreamassoc> \r\n" \
"   <participantstreamassoc participant_id=\"DyxVShKXEeaAO/FyJSj9YQ==\"> \r\n" \
"       <send>DyxVShKXEeaAPPFyJSj9YQ==</send> \r\n" \
"       <recv>DyxVShKXEeaAO/FyJSj9YQ==</recv> \r\n" \
"   </participantstreamassoc> \r\n" \
"</recording> \r\n" \
"\r\n"
);

std::string ciscoSdpBody = std::string(
"v=0 \r\n" \
"o=CiscoSystemsSIP-GW-UserAgent 1478 8649 IN IP4 192.168.33.1 \r\n" \
"s=SIP Call \r\n" \
"c=IN IP4 192.168.33.1 \r\n" \
"t=0 0 \r\n" \
"m=audio 16412 RTP/AVP 8 19 \r\n" \
"c=IN IP4 192.168.33.1 \r\n" \
"a=rtpmap:8 PCMA/8000 \r\n" \
"a=rtpmap:19 CN/8000 \r\n" \
"a=ptime:20 \r\n" \
"a=sendonly \r\n" \
"a=label:3 \r\n" \
"m=audio 16414 RTP/AVP 8 19 \r\n" \
"c=IN IP4 192.168.33.1 \r\n" \
"a=rtpmap:8 PCMA/8000 \r\n" \
"a=rtpmap:19 CN/8000 \r\n" \
"a=ptime:20 \r\n" \
"a=sendonly \r\n" \
"a=label:4 \r\n" \
"\r\n"
);

std::string broadsoftSiprecInvite = std::string(
"INVITE sip:192.168.33.10:5060 SIP/2.0\r\n" \
        "Via: SIP/2.0/UDP 192.168.33.10:5060;branch=z9hG4bKm01f7b00e83gqoo6b7p0.1\r\n" \
        "From: <sip:+442077705371@192.168.33.10;user=phone>;tag=1176615561-1466588683418-\r\n" \
        "To: <sip:192.168.33.10:5060>\r\n" \
        "Call-ID: BW10444341822061696912073@192.168.33.1\r\n" \
        "CSeq: 1002417742 INVITE\r\n" \
        "Contact: <sip:+442077705371@192.168.33.10:5060;transport=udp>;src\r\n" \
        "Privacy: none\r\n" \
        "Recv-Info: \r\n" \
        "Accept: application/media_control+xml,application/sdp,application/x-broadworks-call-center+xml\r\n" \
        "Allow: ACK,BYE,CANCEL,INFO,INVITE,OPTIONS,PRACK,REFER,NOTIFY\r\n" \
        "Supported: \r\n" \
        "Max-Forwards: 54\r\n" \
        "Content-Type: multipart/mixed;boundary=uniqueBoundary\r\n" \
        "Content-Length: 2464\r\n" \
        "MIME-Version: 1.0\r\n" \
        "\r\n" \
        "--uniqueBoundary\r\n" \
        "Content-Type:application/rs-metadata+xml\r\n" \
        "Content-Disposition:recording-session\r\n" \
        "Content-Length:2021\r\n" \
        "\r\n" \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?><recording_metadata xmlns=\"urn:ietf:params:xml:ns:siprec\"><dataMode>complete</dataMode><recording id=\"urn:uuid:b05ab0c2-f62e-42e0-89fb-1bc1112c32b7\"><requestor>SRC</requestor><type>selective</type></recording><group id=\"urn:uuid:9529b910-87b6-4299-a6fe-350f0dcb1f5e\" recording=\"urn:uuid:b05ab0c2-f62e-42e0-89fb-1bc1112c32b7\"><initiator>sip:02077705371</initiator></group><session id=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\" group=\"urn:uuid:9529b910-87b6-4299-a6fe-350f0dcb1f5e\"><start-time>2016-06-22T10:44:43+01:00</start-time></session><participant id=\"urn:uuid:91b39d20-5fe4-4ae1-8553-abc98ce37332\" session=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><aor>sip:02077705371</aor><send><id>urn:uuid:b1ced587-d73d-42a5-8f82-0594d45cbf73</id></send></participant><participant id=\"urn:uuid:b5dea339-d1ca-45f3-97b8-782b5b867701\" session=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><aor>sip:07949538036@exponential-e.com;user=phone</aor><send><id>urn:uuid:b1ced587-d73d-42a5-8f82-0594d45cbf73</id></send></participant><stream id=\"urn:uuid:b1ced587-d73d-42a5-8f82-0594d45cbf73\" session=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><label>1</label><mode>mixed</mode></stream><extensiondata id=\"urn:uuid:3c7b23b3-1ea0-4da4-bfc1-e468297f5074\" parent=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><broadWorksRecordingMetadata xmlns=\"http://schema.broadsoft.com/broadworksCallRecording\" version=\"1.0\"><extTrackingID>8028549:1</extTrackingID><serviceProviderID>HTE039288</serviceProviderID><groupID>EC3V 4AB London</groupID><userID>02077705371@exponential-e.com</userID><callID>callhalf-7948894825:0</callID><callType><origCall><callingPartyNumber>sip:+442077705371@exponential-e.com</callingPartyNumber><calledPartyNumber>sip:07949538036@exponential-e.com;user=phone</calledPartyNumber><dialedDigits>sip:07949538036@exponential-e.com</dialedDigits></origCall></callType><recordingType>on</recordingType></broadWorksRecordingMetadata></extensiondata></recording_metadata>\r\n" \
        "\r\n" \
        "--uniqueBoundary\r\n" \
        "Content-Type: application/sdp\r\n" \
        "Content-Length: 232\r\n" \
        "\r\n" \
        "v=0\r\n" \
        "o=BroadWorks 1419269982 1 IN IP4 192.168.33.1\r\n" \
        "s=-\r\n" \
        "c=IN IP4 192.168.33.1\r\n" \
        "t=0 0\r\n" \
        "m=audio 58014 RTP/AVP 8 101\r\n" \
        "a=rtpmap:8 PCMA/8000\r\n" \
        "a=rtpmap:101 telephone-event/8000\r\n" \
        "a=fmtp:101 0-15\r\n" \
        "a=ptime:20\r\n" \
        "a=sendonly\r\n" \
        "a=label:5\r\n" \
        "\r\n" \
        "--uniqueBoundary--\r\n" \
        "\r\n");

std::string broadsoftSiprecBody = std::string(
        "--uniqueBoundary\r\n" \
        "Content-Type:application/rs-metadata+xml\r\n" \
        "Content-Disposition:recording-session\r\n" \
        "Content-Length:2021\r\n" \
        "\r\n" \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?><recording_metadata xmlns=\"urn:ietf:params:xml:ns:siprec\"><dataMode>complete</dataMode><recording id=\"urn:uuid:b05ab0c2-f62e-42e0-89fb-1bc1112c32b7\"><requestor>SRC</requestor><type>selective</type></recording><group id=\"urn:uuid:9529b910-87b6-4299-a6fe-350f0dcb1f5e\" recording=\"urn:uuid:b05ab0c2-f62e-42e0-89fb-1bc1112c32b7\"><initiator>sip:02077705371</initiator></group><session id=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\" group=\"urn:uuid:9529b910-87b6-4299-a6fe-350f0dcb1f5e\"><start-time>2016-06-22T10:44:43+01:00</start-time></session><participant id=\"urn:uuid:91b39d20-5fe4-4ae1-8553-abc98ce37332\" session=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><aor>sip:02077705371</aor><send><id>urn:uuid:b1ced587-d73d-42a5-8f82-0594d45cbf73</id></send></participant><participant id=\"urn:uuid:b5dea339-d1ca-45f3-97b8-782b5b867701\" session=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><aor>sip:07949538036@exponential-e.com;user=phone</aor><send><id>urn:uuid:b1ced587-d73d-42a5-8f82-0594d45cbf73</id></send></participant><stream id=\"urn:uuid:b1ced587-d73d-42a5-8f82-0594d45cbf73\" session=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><label>1</label><mode>mixed</mode></stream><extensiondata id=\"urn:uuid:3c7b23b3-1ea0-4da4-bfc1-e468297f5074\" parent=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><broadWorksRecordingMetadata xmlns=\"http://schema.broadsoft.com/broadworksCallRecording\" version=\"1.0\"><extTrackingID>8028549:1</extTrackingID><serviceProviderID>HTE039288</serviceProviderID><groupID>EC3V 4AB London</groupID><userID>02077705371@exponential-e.com</userID><callID>callhalf-7948894825:0</callID><callType><origCall><callingPartyNumber>sip:+442077705371@exponential-e.com</callingPartyNumber><calledPartyNumber>sip:07949538036@exponential-e.com;user=phone</calledPartyNumber><dialedDigits>sip:07949538036@exponential-e.com</dialedDigits></origCall></callType><recordingType>on</recordingType></broadWorksRecordingMetadata></extensiondata></recording_metadata>\r\n" \
        "\r\n" \
        "--uniqueBoundary\r\n" \
        "Content-Type: application/sdp\r\n" \
        "Content-Length: 232\r\n" \
        "\r\n" \
        "v=0\r\n" \
        "o=BroadWorks 1419269982 1 IN IP4 192.168.33.1\r\n" \
        "s=-\r\n" \
        "c=IN IP4 192.168.33.1\r\n" \
        "t=0 0\r\n" \
        "m=audio 58014 RTP/AVP 8 101\r\n" \
        "a=rtpmap:8 PCMA/8000\r\n" \
        "a=rtpmap:101 telephone-event/8000\r\n" \
        "a=fmtp:101 0-15\r\n" \
        "a=ptime:20\r\n" \
        "a=sendonly\r\n" \
        "a=label:5\r\n" \
        "\r\n" \
        "--uniqueBoundary--\r\n" \
        "\r\n");

std::string broadsoftXmlBody = std::string(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?><recording_metadata xmlns=\"urn:ietf:params:xml:ns:siprec\"><dataMode>complete</dataMode><recording id=\"urn:uuid:b05ab0c2-f62e-42e0-89fb-1bc1112c32b7\"><requestor>SRC</requestor><type>selective</type></recording><group id=\"urn:uuid:9529b910-87b6-4299-a6fe-350f0dcb1f5e\" recording=\"urn:uuid:b05ab0c2-f62e-42e0-89fb-1bc1112c32b7\"><initiator>sip:02077705371</initiator></group><session id=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\" group=\"urn:uuid:9529b910-87b6-4299-a6fe-350f0dcb1f5e\"><start-time>2016-06-22T10:44:43+01:00</start-time></session><participant id=\"urn:uuid:91b39d20-5fe4-4ae1-8553-abc98ce37332\" session=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><aor>sip:02077705371</aor><send><id>urn:uuid:b1ced587-d73d-42a5-8f82-0594d45cbf73</id></send></participant><participant id=\"urn:uuid:b5dea339-d1ca-45f3-97b8-782b5b867701\" session=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><aor>sip:07949538036@exponential-e.com;user=phone</aor><send><id>urn:uuid:b1ced587-d73d-42a5-8f82-0594d45cbf73</id></send></participant><stream id=\"urn:uuid:b1ced587-d73d-42a5-8f82-0594d45cbf73\" session=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><label>5</label><mode>mixed</mode></stream><extensiondata id=\"urn:uuid:3c7b23b3-1ea0-4da4-bfc1-e468297f5074\" parent=\"urn:uuid:08d46cef-4858-4658-8388-ebb7ad4b1dab\"><broadWorksRecordingMetadata xmlns=\"http://schema.broadsoft.com/broadworksCallRecording\" version=\"1.0\"><extTrackingID>8028549:1</extTrackingID><serviceProviderID>HTE039288</serviceProviderID><groupID>EC3V 4AB London</groupID><userID>02077705371@exponential-e.com</userID><callID>callhalf-7948894825:0</callID><callType><origCall><callingPartyNumber>sip:+442077705371@exponential-e.com</callingPartyNumber><calledPartyNumber>sip:07949538036@exponential-e.com;user=phone</calledPartyNumber><dialedDigits>sip:07949538036@exponential-e.com</dialedDigits></origCall></callType><recordingType>on</recordingType></broadWorksRecordingMetadata></extensiondata></recording_metadata>\r\n" \
        "\r\n");

std::string broadsoftSdpBody = std::string(
        "v=0\r\n" \
        "o=BroadWorks 1419269982 1 IN IP4 192.168.33.1\r\n" \
        "s=-\r\n" \
        "c=IN IP4 192.168.33.1\r\n" \
        "t=0 0\r\n" \
        "m=audio 58014 RTP/AVP 8 101\r\n" \
        "a=rtpmap:8 PCMA/8000\r\n" \
        "a=rtpmap:101 telephone-event/8000\r\n" \
        "a=fmtp:101 0-15\r\n" \
        "a=ptime:20\r\n" \
        "a=sendonly\r\n" \
        "a=label:5\r\n" \
        "\r\n");


struct sockaddr_in destinationAddress;
uv_udp_t udpClientSocket;

void on_send(struct uv_udp_send_s * req, int status) {
    fprintf(stderr, "onUdpSend, status=%d\n", status);
//    for (int i = 0; i < req->nbufs; ++i) {
//        fprintf(stderr, "onUdpRecv need to purge buffer %d\n", i);
//    }
}

int sendMessage(
        const char *remoteSipAddress,
        int remoteSipPort,
        string  message) {

    int status;
    uv_buf_t *buf;
    uv_udp_send_t *req;

    fprintf(stderr, "Cisco Siprec body has length %ld\n", ciscoSiprecBody.size());
    fprintf(stderr, "Broadsoft Siprec body has length %ld\n", broadsoftSiprecBody.size());

    uv_udp_init(uv_default_loop(), &udpClientSocket);

    fprintf(stderr, "TX ===>>>\n%s:%d\n%s\n", remoteSipAddress, remoteSipPort, message.c_str());

    uv_ip4_addr(remoteSipAddress, remoteSipPort, &destinationAddress);

    buf = (uv_buf_t *)malloc(sizeof(uv_buf_t));
    buf->len = message.size();

    buf->base = (char *)malloc(buf->len);
    //memset(buf->base, 0, buf->len);
    memcpy(buf->base, message.c_str(), buf->len);

    req = (uv_udp_send_t *)malloc(sizeof(uv_udp_send_t));

    status = uv_udp_send(req, &udpClientSocket, buf, 1, (const sockaddr *)&destinationAddress, &on_send);

    if (status) {
        fprintf(stderr, "SipInterface::sendMessage send returned %d, %s\n", status, uv_err_name(status));
    } else {
        fprintf(stderr, "SipInterface::sendMessage sent OK\n");
    }

    return status;
}

void background_process(void * data) {
    fprintf(stderr, "background_process Started\n");

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    fprintf(stderr, "background_process Stopped\n");
}

void idle_handler(uv_idle_t* handle) {
    //fprintf(stderr, "Idle Handler event\n");
}

int main(int argc, char * argv[]) {

//    fprintf(stderr, "Broadsoft Siprec body has length %ld\n", broadsoftSiprecBody.size());
//    fprintf(stderr, "Broadsoft XML body has length %ld\n", broadsoftXmlBody.size());
//    fprintf(stderr, "Broadsoft SDP body has length %ld\n", broadsoftSdpBody.size());
//    fprintf(stdout, "%s", broadsoftSiprecInvite.c_str());

    fprintf(stderr, "Cisco Siprec body has length %ld\n", ciscoSiprecBody.size());
    fprintf(stderr, "Cisco XML body has length %ld\n", ciscoXmlBody.size());
    fprintf(stderr, "Cisco SDP body has length %ld\n", ciscoSdpBody.size());
    fprintf(stdout, "%s", ciscoSiprecInvite.c_str());

    return 0;
//
//    uv_thread_t loop_thread;
//
//    uv_idle_t idler;
//
//    uv_idle_init(uv_default_loop(), &idler);
//    uv_idle_start(&idler, idle_handler);
//
//    uv_thread_create(&loop_thread, background_process, 0);
//
//    fprintf(stderr, "Started\n");
//
//
//
//    sendMessage("192.168.33.10", 5060, ciscoSiprecInvite);
//    //sendMessage("192.168.0.205", 5060, sampleSipInvite);
//
//
//    getc(stdin);
//
//    fprintf(stderr, "Stopping\n");
//
//    uv_stop(uv_default_loop());
//
//    uv_thread_join(&loop_thread);
}
