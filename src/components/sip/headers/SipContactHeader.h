//
// Created by Simon Willis on 23/05/2016.
//

#ifndef CORE_SIPCONTACTHEADER_H
#define CORE_SIPCONTACTHEADER_H


#include "SipHeader.h"

class SipContactHeader : public SipHeader {

public:

    //TODO Implement specifics of Contact Header

    /*
   From https://tools.ietf.org/html/rfc3261#section-20.10

   A Contact header field value provides a URI whose meaning depends on
   the type of request or response it is in.

   A Contact header field value can contain a display name, a URI with
   URI parameters, and header parameters.

   This document defines the Contact parameters "q" and "expires".
   These parameters are only used when the Contact is present in a
   REGISTER request or response, or in a 3xx response.  Additional
   parameters may be defined in other specifications.

   When the header field value contains a display name, the URI
   including all URI parameters is enclosed in "<" and ">".  If no "<"
   and ">" are present, all parameters after the URI are header
   parameters, not URI parameters.  The display name can be tokens, or a
   quoted string, if a larger character set is desired.

   Even if the "display-name" is empty, the "name-addr" form MUST be
   used if the "addr-spec" contains a comma, semicolon, or question
   mark.  There may or may not be LWS between the display-name and the
   "<".

   These rules for parsing a display name, URI and URI parameters, and
   header parameters also apply for the header fields To and From.

      The Contact header field has a role similar to the Location header
      field in HTTP.  However, the HTTP header field only allows one
      address, unquoted.  Since URIs can contain commas and semicolons
      as reserved characters, they can be mistaken for header or
      parameter delimiters, respectively.

   The compact form of the Contact header field is m (for "moved").

   Examples:

      Contact: "Mr. Watson" <sip:watson@worcester.bell-telephone.com>
         ;q=0.7; expires=3600,
         "Mr. Watson" <mailto:watson@bell-telephone.com> ;q=0.1
      m: <sips:bob@192.0.2.4>;expires=60

     */

    SipContactHeader(mapped_buffer_t headerValue)
            : SipHeader(headerValue) {

    }

    virtual const char *getFieldName() override {
        return "Contact";
    }

    virtual SipHeaderType getType() override {
        return SipHeaderType::CONTACT;
    }

};


#endif //CORE_SIPCONTACTHEADER_H
