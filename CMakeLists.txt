cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++0x)

include_directories("lib/")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/bin/")

add_library( uv SHARED STATIC IMPORTED )

if(APPLE)
    set_target_properties( uv PROPERTIES IMPORTED_LOCATION /usr/local/lib/libuv.dylib )
else()
    set_target_properties( uv PROPERTIES IMPORTED_LOCATION /usr/lib64/libuv.so )
endif()

set(UTILITY_SOURCE_FILES
        lib/json/json.h
        lib/json/jsoncpp.cpp
        src/components/utilities/buffers.h
        src/components/utilities/buffers.cpp
        src/components/utilities/base64.h
        src/components/utilities/base64.cpp
        src/components/utilities/util.hpp
        src/components/utilities/util.cpp
        src/components/utilities/MessageBuffer.cpp
        src/components/utilities/MessageBuffer.h
        src/components/utilities/XmlDocument.cpp
        src/components/utilities/XmlDocument.h
        )

set(NETWORKING_SOURCE_FILES
        src/components/utilities/SocketServer.cpp
        src/components/utilities/SocketServer.h)

set(SIP_SOURCE_FILES
        src/components/sip/SipInterface.h
        src/components/sip/SipInterface.cpp
        src/components/sip/headers/SipHeader.cpp
        src/components/sip/headers/SipHeader.h
        src/components/sip/headers/SipAcceptHeader.h
        src/components/sip/headers/SipCallIdHeader.h
        src/components/sip/headers/SipContactHeader.h
        src/components/sip/headers/SipContentDispositionHeader.h
        src/components/sip/headers/SipContentEncodingHeader.h
        src/components/sip/headers/SipContentTypeHeader.h
        src/components/sip/headers/SipContentLengthHeader.h
        src/components/sip/headers/SipCSeqHeader.h
        src/components/sip/headers/SipDateHeader.h
        src/components/sip/headers/SipFromHeader.h
        src/components/sip/headers/SipMaxForwardsHeader.h
        src/components/sip/headers/SipAllowHeader.h
        src/components/sip/headers/SipRequireHeader.h
        src/components/sip/headers/SipEmptyHeader.h
        src/components/sip/headers/SipRequireAfterHeader.h
        src/components/sip/headers/SipSupportedHeader.h
        src/components/sip/headers/SipTimestampHeader.h
        src/components/sip/headers/SipToHeader.h
        src/components/sip/headers/SipUnsupportedHeader.h
        src/components/sip/headers/SipUserAgentHeader.h
        src/components/sip/headers/SipViaHeader.h
        src/components/sip/headers/SipMinSeHeader.h
        src/components/sip/headers/SipExtendedHeader.h
        src/components/sip/headers/SipAssertedIdentityHeader.h
        src/components/sip/headers/SipExpiresHeader.h
        src/components/sip/headers/SipAllowEventsHeader.h
        src/components/sip/headers/SipMimeVersionHeader.h

        src/components/sip/events/CallEvent.h
        src/components/sip/events/CallNewCallEvent.h
        src/components/sip/events/CallAnsweredEvent.h
        src/components/sip/events/CallClearedEvent.h
        src/components/sip/events/CallMediaErrorEvent.h
        src/components/sip/events/CallMediaStatsEvent.h

        src/components/sip/messages/SipMessage.cpp
        src/components/sip/messages/SipMessage.h

        src/components/sip/messages/SipRequest.h
        src/components/sip/messages/SipInviteRequest.h
        src/components/sip/messages/SipAckRequest.h
        src/components/sip/messages/SipByeRequest.h
        src/components/sip/messages/SipCancelRequest.h
        src/components/sip/messages/SipUnknownRequest.h
        src/components/sip/messages/SipOptionsRequest.h

        src/components/sip/messages/SipResponse.h
        src/components/sip/messages/SipTryingResponse.h

        src/components/sip/SipTransaction.h
        src/components/sip/messages/SipRequest.cpp
        src/components/sip/messages/SipRingingResponse.h
        src/components/sip/messages/SipOkResponse.h
        src/components/sip/messages/SipSystemUnavailableResponse.h
        src/components/sip/SipDialogueManager.cpp
        src/components/sip/SipDialogueManager.h
        src/components/sip/SipDialogue.cpp
        src/components/sip/SipDialogue.h
        src/components/sip/SipParser.cpp
        src/components/sip/SipParser.h
        src/components/sip/messages/SipMessageBody.h
        src/components/sip/messages/SipInviteRequest.cpp
        src/components/sip/headers/SipReasonHeader.h
        src/components/sip/headers/SipSubjectHeader.h
        src/components/sip/events/CallMediaRecordingStartedEvent.h
        src/components/sip/events/CallMediaRecordingStoppedEvent.h
        src/components/sip/SipConnection.hpp
        )

set (CONTROL_SOURCE_FILES
        src/components/control/requests/Request.cpp
        src/components/control/requests/Request.h
        src/components/control/requests/LoginRequest.h
        src/components/control/requests/CallCountRequest.h
        src/components/control/requests/CallAnswerRequest.h
        src/components/control/requests/CallRecordRequest.h
        src/components/control/requests/CallClearRequest.h
        src/components/control/requests/CallRingRequest.h
        src/components/control/requests/CallProgressRequest.h
        src/components/control/requests/CallInitiateRequest.h
        src/components/control/notifications/Notification.hpp
        src/components/control/notifications/LoginNotification.h
        src/components/control/notifications/CallCountNotification.h
        src/components/control/notifications/CallIncomingNotification.h
        src/components/control/notifications/CallProgressNotification.h
        src/components/control/notifications/CallRingingNotification.h
        src/components/control/notifications/CallConnectedNotification.h
        src/components/control/notifications/CallClearedNotification.h
        src/components/control/notifications/CallRecordingStartedNotification.h
        src/components/control/notifications/CallRecordingCompletedNotification.h
        src/components/control/notifications/CallStatisticsNotification.h
        )

set(CORE_SOURCE_FILES
        src/components/control/ControlInterface.h
        src/components/control/ControlInterface.cpp
        src/components/media/data/RtpPacket.hpp src/components/media/session/MediaSession.cpp)

set(MEDIA_SOURCE_FILES
        src/components/media/messages/MediaMessage.cpp
        src/components/media/messages/MediaMessage.h
        src/components/media/messages/MediaRequest.h
        src/components/media/messages/MediaResponse.h
        src/components/media/messages/MediaAllocateRequest.h
        src/components/media/messages/MediaRecordStartRequest.h
        src/components/media/messages/MediaRecordStopRequest.h
        src/components/media/messages/MediaRecordStartResponse.h
        src/components/media/messages/MediaRecordStopResponse.h
        src/components/media/messages/MediaReleaseRequest.h
        src/components/media/messages/MediaReleaseResponse.h
        src/components/media/messages/MediaAllocateResponse.h
        src/components/media/messages/MediaRegisterRequest.h
        src/components/media/messages/MediaRegisterResponse.h
        src/components/media/MediaClientInterface.cpp
        src/components/media/MediaClientInterface.h
        src/components/media/MediaServerInterface.cpp
        src/components/media/MediaServerInterface.h
        src/components/media/messages/MediaPlayStartRequest.h
        src/components/media/messages/MediaPlayStopRequest.h
        src/components/media/definitions.h
        src/components/media/resource/Device.h
        src/components/media/resource/MediaResource.cpp
        src/components/media/resource/MediaResource.h
        src/components/media/resource/AllocatedResource.cpp
        src/components/media/resource/AllocatedResource.h
        src/components/media/resource/ResourceManager.cpp
        src/components/media/resource/ResourceManager.h
        src/components/sdp/Sdp.cpp
        src/components/sdp/Sdp.h
        src/components/sdp/SdpOriginLine.hpp
        src/components/sdp/SdpAttributeLine.hpp
        src/components/sdp/SdpConnectionLine.hpp
        src/components/sdp/SdpMediaLine.hpp
        src/components/sdp/SdpTiming.hpp
        src/components/sdp/Sdp.cpp
        src/components/sdp/Sdp.h

        src/components/sdp/SdpAddrType.hpp
        src/components/sdp/SdpMediaType.hpp
        src/components/sdp/SdpProtocolType.hpp
        src/components/sdp/SdpFormatType.hpp
        src/components/sdp/SdpMedia.hpp
        src/components/sdp/SdpAttribute.hpp
        src/components/sdp/SdpAttributeType.hpp
        src/components/sdp/SdpFormat.hpp
        src/components/sdp/SdpOrigin.hpp
        src/components/sdp/SdpConnection.hpp
        src/components/media/data/RtpPacket.hpp
        src/components/media/data/RtcpPacket.hpp
        src/components/media/session/MediaSession.h
        src/components/media/session/MediaSessionManager.cpp
        src/components/media/session/MediaSessionManager.h
        src/components/sdp/SdpSession.hpp
        )


project(Media)
add_executable(Media
        src/applications/media.cpp
        ${MEDIA_SOURCE_FILES}
        ${UTILITY_SOURCE_FILES}
        src/components/media/session/MediaSession.cpp)
target_link_libraries(Media uv)

project(Control)
add_executable(Control
        src/applications/control.cpp
        ${CONTROL_SOURCE_FILES}
        ${UTILITY_SOURCE_FILES}
        )
target_link_libraries(Control uv)

project(TestSipParsing)
add_executable(TestSipParsing
        tests/TestSipParsing.cpp
        ${SIP_SOURCE_FILES}
        ${UTILITY_SOURCE_FILES}
        tests/data/SipMessageSamples.hpp
        )
target_link_libraries(TestSipParsing uv)

project(TestMappedBuffers)
add_executable(TestMappedBuffers
        tests/TestMappedBuffers.cpp
        ${UTILITY_SOURCE_FILES}
        )

project(TestSessionManager)
add_executable(TestSessionManager
        tests/TestSessionManager.cpp
        ${SIP_SOURCE_FILES}
        ${UTILITY_SOURCE_FILES}
        tests/data/SipMessageSamples.hpp)
target_link_libraries(TestSessionManager uv)

project(TestSocketServer)
add_executable(TestSocketServer
        tests/TestSocketServer.cpp
        ${NETWORKING_SOURCE_FILES}
        ${UTILITY_SOURCE_FILES}
        )
target_link_libraries(TestSocketServer uv)

project(TestMessageBuffer)
add_executable(TestMessageBuffer
        tests/TestMessageBuffer.cpp
        ${UTILITY_SOURCE_FILES}
        )

project(TestSdpClass)
add_executable(TestSdpClass
        tests/TestSdpClass.cpp
        ${MEDIA_SOURCE_FILES}
        ${UTILITY_SOURCE_FILES}
        )
target_link_libraries(TestSdpClass uv)

project(TestXmlDocumentClass)
add_executable(TestXmlDocumentClass
        tests/TestXmlDocumentClass.cpp
        ${UTILITY_SOURCE_FILES}
        )

project(TestUtil)
add_executable(TestUtil
        tests/TestUtil.cpp
        ${UTILITY_SOURCE_FILES}
        )

project(UdpSend)
add_executable(UdpSend
        tools/UdpSend.cpp
        )
target_link_libraries(UdpSend uv)

project(Core)
add_executable(Core
        src/applications/core.cpp
        ${CORE_SOURCE_FILES}
        ${UTILITY_SOURCE_FILES}
        ${SIP_SOURCE_FILES}
        ${CONTROL_SOURCE_FILES}
        ${MEDIA_SOURCE_FILES}
        )
target_link_libraries(Core uv)




