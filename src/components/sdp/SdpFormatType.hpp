//
// Created by Simon Willis on 15/05/2017.
//

#ifndef SDPFORMATDATA_H
#define SDPFORMATDATA_H

#include <cstdio>
#include <cstdint>
#include <string>

class SdpFormatType {

public:

    enum Fmt {
        PCMU = 0,
        G723 = 4,
        PCMA = 8,
        G722 = 9,
        G729 = 18,
        DTMF = 101,
        CUSTOM = 120 // includes codecs such as OPUS - requires fmtp attributes
    };

public:

    SdpFormatType() : fmt(PCMU) { }

    SdpFormatType(Fmt fmt) : fmt(fmt) { }

    SdpFormatType(uint16_t fmtId) { setFmt(fmtId); }

    SdpFormatType(const SdpFormatType &other) : fmt(other.fmt) { }

public:

    std::string toString() {
        switch(fmt) {
            case PCMU: return "PCMU";
            case G723: return "G723";
            case PCMA: return "PCMA";
            case G729: return "G729";
            case G722: return "G722";
            case DTMF: return "DTMF";
            case CUSTOM: return "CUSTOM";
        }
    }

    void setFmt(uint16_t fmtId) {
        switch (fmtId) {
            case PCMU: fmt = PCMU; break;
            case PCMA: fmt = PCMA; break;
            case G729: fmt = G729; break;
            case G722: fmt = G722; break;
            case DTMF: fmt = DTMF; break;
            case CUSTOM: fmt = CUSTOM; break;
        }
    }

    uint16_t getFmtNumber() {
        return (uint16_t)fmt;
    }

private:

    Fmt fmt;

};

#endif //SDPFORMATDATA_H
