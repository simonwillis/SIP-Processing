//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPHEADER_H
#define CORE_SIPHEADER_H


#include <string>
#include <list>
#include "../../utilities/buffers.h"

//TODO Rationalise SipHeader classes and the name value collection typdefs

typedef const std::string sip_header_name_t;
typedef const std::string sip_header_value_t;
typedef std::pair<sip_header_name_t, sip_header_value_t> sip_header_t;
typedef std::list<sip_header_t> sip_headers_t;

class SipHeader {

public:
    enum class SipHeaderType {
        UNKNOWN = 0,
        EMPTY,
        VIA,
        FROM,
        TO,
        CALL_ID,
        DATE,
        SUPPORTED,
        UNSUPPORTED,
        ACCEPT,
        REQUIRE,
        REQUIRE_AFTER,
        MINSE,
        USER_AGENT,
        ALLOW,
        AllOW_EVENTS,
        CSEQ,
        MAX_FORWARDS,
        SUBJECT,
        TIMESTAMP,
        CONTACT,
        EXPIRES,
        REASON,
        CONTENT_TYPE,
        CONTENT_LENGTH,
        MIME_VERSION,
        CONTENT_ENCODING,
        CONTENT_DISPOSITION,
        P_ASSERTED_IDENTITY,
        EXTENDED
    };

    SipHeader(mapped_buffer_t value);
    virtual const char * getFieldName() = 0;
    virtual SipHeaderType getType() = 0;
    virtual bool isExtended() { return false; };
    const int getIntValue() { return toLongInteger(headerValue); }
    sip_header_t getDescriptor() { return std::make_pair<sip_header_name_t, sip_header_value_t>(std::string(getFieldName()), headerValue.toString()); }

protected:
    mapped_buffer_t headerValue;
};




#endif //CORE_SIPHEADER_H
