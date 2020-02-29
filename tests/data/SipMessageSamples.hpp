//
// Created by simon on 30/07/16.
//

#ifndef TEST_SIP_MESSAGE_SAMPLE_DATA_HPP
#define TEST_SIP_MESSAGE_SAMPLE_DATA_HPP


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


std::string sampleSdpOffer = std::string (
        "v=0\r\n" \
"o=CiscoSystemsSIP-GW-UserAgent 1478 8649 IN IP4 172.16.1.20\r\n" \
"s=SIP Call\r\n" \
"c=IN IP4 172.16.1.20\r\n" \
"t=0 0\r\n" \
"m=audio 16412 RTP/AVP 8 19\r\n" \
"c=IN IP4 172.16.1.20\r\n" \
"a=rtpmap:8 PCMA/8000\r\n" \
"a=rtpmap:19 CN/8000\r\n" \
"a=ptime:20\r\n" \
"a=sendonly\r\n" \
"a=label:1\r\n" \
"m=audio 16414 RTP/AVP 8 19\r\n" \
"c=IN IP4 172.16.1.20\r\n" \
"a=rtpmap:8 PCMA/8000\r\n" \
"a=rtpmap:19 CN/8000\r\n" \
"a=ptime:20\r\n" \
"a=sendonly\r\n" \
"a=label:2\r\n" \
"\r\n");

std::string inviteWithUnusualBody = std::string (
        "INVITE sip:user@example.com SIP/2.0\r\n" \
"Contact: <sip:caller@host5.example.net>\r\n" \
"To: sip:j.user@example.com\r\n" \
"From: sip:caller@example.net;tag=8392034\r\n" \
"Max-Forwards: 70\r\n" \
"Call-ID: invut.0ha0isndaksdjadsfij34n23d\r\n" \
"CSeq: 235448 INVITE\r\n" \
"Via: SIP/2.0/UDP somehost.example.com;branch=z9hG4bKkdjuw\r\n" \
"Content-Type: application/unknownformat\r\n" \
"Content-Length: 40\r\n" \
"\r\n" \
"<audio>\r\n" \
"<pcmu port=\"443\"/>\r\n" \
"</audio>\r\n" \
"\r\n");

std::string standardInviteWithSdp = std::string (
        "INVITE sip:bob@biloxi.example.com SIP/2.0\r\n" \
"Via: SIP/2.0/TCP client.atlanta.example.com:5060;branch=z9hG4bK74bf9\r\n" \
"Max-Forwards: 70\r\n" \
"From: Alice <sip:alice@atlanta.example.com>;tag=9fxced76sl\r\n" \
"To: Bob <sip:bob@biloxi.example.com>\r\n" \
"Call-ID: 3848276298220188511@atlanta.example.com\r\n" \
"CSeq: 1 INVITE\r\n" \
"Contact: <sip:alice@client.atlanta.example.com;transport=tcp>\r\n" \
"Content-Type: application/sdp\r\n" \
"Content-Length: 151\r\n" \
"\r\n" \
"v=0\r\n" \
"o=alice 2890844526 2890844526 IN IP4 client.atlanta.example.com\r\n" \
"s=-\r\n" \
"c=IN IP4 192.0.2.101\r\n" \
"t=0 0\r\n" \
"m=audio 49172 RTP/AVP 0\r\n" \
"a=rtpmap:0 PCMU/8000\r\n"
);



std::string standardAckMessage = std::string (
        "ACK sip:bob@client.biloxi.example.com SIP/2.0\r\n" \
"Via: SIP/2.0/TCP client.atlanta.example.com:5060;branch=z9hG4bK74bd5\r\n" \
"Max-Forwards: 70\r\n" \
"From: Alice <sip:alice@atlanta.example.com>;tag=9fxced76sl\r\n" \
"To: Bob <sip:bob@biloxi.example.com>;tag=8321234356\r\n" \
"Call-ID: 3848276298220188511@atlanta.example.com\r\n" \
"CSeq: 1 ACK\r\n" \
"Content-Length: 0\r\n" \
"\r\n"
);

std::string standardTryingResponse = std::string (
        "SIP/2.0 100 Trying\r\n" \
"Via: SIP/2.0/UDP client.atlanta.example.com:5060;branch=z9hG4bK74bf9\r\n" \
" ;received=192.0.2.101\r\n" \
"From: Alice <sip:alice@atlanta.example.com>;tag=9fxced76sl\r\n" \
"To: Bob <sip:bob@biloxi.example.com>\r\n" \
"Call-ID: 2xTb9vxSit55XU7p8@atlanta.example.com\r\n" \
"CSeq: 1 INVITE\r\n" \
"Content-Length: 0\r\n" \
"\r\n"
);

std::string standardRingingResponse = std::string (
        "SIP/2.0 180 Ringing\r\n" \
"Via: SIP/2.0/TCP client.atlanta.example.com:5060;branch=z9hG4bK74bf9\r\n" \
" ;received=192.0.2.101\r\n" \
"From: Alice <sip:alice@atlanta.example.com>;tag=9fxced76sl\r\n" \
"To: Bob <sip:bob@biloxi.example.com>;tag=8321234356\r\n" \
"Call-ID: 3848276298220188511@atlanta.example.com\r\n" \
"CSeq: 1 INVITE\r\n" \
"Contact: <sip:bob@client.biloxi.example.com;transport=tcp>\r\n" \
"Content-Length: 0\r\n" \
"\r\n"
);

std::string standardByeMessage = std::string (
        "BYE sip:alice@client.atlanta.example.com SIP/2.0\r\n" \
"Via: SIP/2.0/TCP client.biloxi.example.com:5060;branch=z9hG4bKnashds7\r\n" \
"Max-Forwards: 70\r\n" \
"From: Bob <sip:bob@biloxi.example.com>;tag=8321234356\r\n" \
"To: Alice <sip:alice@atlanta.example.com>;tag=9fxced76sl\r\n" \
"Call-ID: 3848276298220188511@atlanta.example.com\r\n" \
"CSeq: 1 BYE\r\n" \
"Content-Length: 0\r\n" \
"\r\n"
);

std::string standarOkToByeResponse = std::string (
        "SIP/2.0 200 OK\r\n" \
"Via: SIP/2.0/TCP client.biloxi.example.com:5060;branch=z9hG4bKnashds7\r\n" \
" ;received=192.0.2.201\r\n" \
"From: Bob <sip:bob@biloxi.example.com>;tag=8321234356\r\n" \
"To: Alice <sip:alice@atlanta.example.com>;tag=9fxced76sl\r\n" \
"Call-ID: 3848276298220188511@atlanta.example.com\r\n" \
"CSeq: 1 BYE\r\n" \
"Content-Length: 0\r\n" \
"\r\n"
);








#endif //TEST_SIP_MESSAGE_SAMPLE_DATA_HPP
