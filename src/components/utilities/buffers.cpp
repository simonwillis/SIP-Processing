
#include <cstdlib>
#include <sstream>

#include "buffers.h"

bool buffer_equals(mapped_buffer_t buffer, const char * value, bool caseSensitivity) {
    size_t index = 0;
    char * p = (char *)buffer.buf;
    char * v = (char *)value;
    if (v == 0) return false;
    if (*v == 0) return (buffer.len == 0);
    for ( ; index < buffer.len; ++p, ++v, ++index) {
        if (caseSensitivity) {
            if (*v == 0 || *p != *v) return false;
        } else {
            if (*v == 0 || toupper(*p) != toupper(*v)) return false;
        }
    }
    return (value[index] == 0 // confirm that the value does not go on for longer
            && index == buffer.len);
}

bool buffer_equals(mapped_buffer_t buffer, mapped_buffer_t value, bool caseSensitivity) {
    if (buffer.len != value.len) return false;
    size_t index = 0;
    char * p = (char *)buffer.buf;
    char * v = (char *)value.buf;
    for (index = 0; index < buffer.len; ++p, ++v, ++index) {
        if (caseSensitivity) {
            if (*p != *v) return false;
        } else {
            if (toupper(*p) != toupper(*v)) return false;
        }
    }
    return true;
};


bool buffer_starts_with(mapped_buffer_t buffer, const char * value, bool caseSensitivity) {
    size_t index = 0;
    char * p = (char *)buffer.buf;
    char * v = (char *)value;
    if (v == 0) return false;
    if (*v == 0) return false; // this differs from 'equals' in that both empty is equal, but ...
    for ( ; index < buffer.len; ++p, ++v, ++index) {
        if (*v == 0) return true; // we got to end of value without failing
        if (caseSensitivity) {
            if (*p != *v) return false;
        } else {
            if (toupper(*p) != toupper(*v)) return false;
        }
    }
    return (value[index] == 0 && index == buffer.len);
}

bool buffer_ends_with(mapped_buffer_t buffer, const char * value, bool caseSensitivity) { ;
    if (!value) return false;
    size_t index = strlen(value);
    if (index == 0) return false;
    if (index > buffer.len) return false;

    char * p = (char *)(buffer.buf + buffer.len - 1);

    char * v = (char *)(value + index - 1);
    for ( ; index != 0; --p, --v, --index) {
        if (caseSensitivity) {
            if (*p != *v) return false;
        } else {
            if (toupper(*p) != toupper(*v)) return false;
        }
    }
    return true;
}

bool buffer_contains(mapped_buffer_t buffer, const char * value, bool caseSensitivity) {
    if (! value) return false;
    size_t valueLength = strlen(value);
    if (valueLength > buffer.len) return false;
    size_t bufferIndex, valueIndex;

    bool result = false;
    char * pB, * pV;

    for (bufferIndex = 0; ((bufferIndex <= (buffer.len - valueLength)) && (result == false)); ++bufferIndex) {
        result = true;
        for (valueIndex = 0; ((valueIndex < valueLength) && (result == true)); ++valueIndex) {
            pB = (char *)(buffer.buf + bufferIndex + valueIndex);
            pV = (char *)(value + valueIndex);
            if (caseSensitivity) {
                if (*pB != *pV) result = false;
            } else {
                if (toupper(*pB) != toupper(*pV)) result = false;
            }
        }
    }
    return result;
}

std::string buffers_join(std::vector<mapped_buffer_t> &buffers, const char delimiter, uint8_t offset) {
    std::stringstream ss;
    size_t size = buffers.size();
    for (int i = offset; i < size; ++i) {
        if (i) {
            ss << delimiter;
        }
        ss << buffers[i].toString();
    }

    return ss.str();
}

unsigned long toUnsignedLongInteger(mapped_buffer_t buffer, int radix) {
    if (buffer.len == 0) {
        return 0;
    }
    char * bufferEnd = (char *)buffer.buf + buffer.len;
    return std::strtoul(buffer.buf, &bufferEnd, radix);
}

uint16_t toUnsignedShortInteger(mapped_buffer_t buffer, int radix) {
    return (uint16_t) toUnsignedLongInteger(buffer, radix);
}

long toLongInteger(mapped_buffer_t buffer, int radix) {
    if (buffer.len == 0) {
        return 0;
    }
    char * bufferEnd = (char *)buffer.buf + buffer.len;
    return std::strtol(buffer.buf, &bufferEnd, radix);
}

std::string bufferToString(mapped_buffer_t buffer) {
    return std::string(buffer.buf, buffer.len);
}

std::vector <mapped_buffer_t> buffer_split_lines(mapped_buffer_t buffer) {
    char * pBuffer = (char *)buffer.buf;
    char * pLine = 0;
    bool readingLineData = false;
    size_t bufferIndex;
    std::vector<mapped_buffer_t> result;

    for (bufferIndex = 0; bufferIndex < buffer.len; ++pBuffer, ++bufferIndex) {
        if (! readingLineData) {
            if (*pBuffer == '\r' || *pBuffer == '\n') {
                continue;
            } else {
                pLine = pBuffer;
                readingLineData = true;
            }
        } else {
            if (*pBuffer == '\r' || *pBuffer == '\n') {
                result.push_back(mapped_buffer_t(pLine, pBuffer - pLine));
                readingLineData = false;
            }
        }
    }

    // The last entry may not have been line terminated in which case we never
    // marked the end of the line and stored the entry. Do it here
    if (readingLineData) {
        result.push_back(mapped_buffer_t(pLine, pBuffer - pLine));
    }

    return result;
}

std::vector <mapped_buffer_t> buffer_split(mapped_buffer_t buffer, char delimiter) {
    char * pBuffer = (char *)buffer.buf;
    char * pStart = pBuffer;
    size_t bufferIndex;
    bool lastWasDelimiter = true;
    std::vector<mapped_buffer_t> result;

    for (bufferIndex = 0; bufferIndex < buffer.len; ++pBuffer, ++bufferIndex) {
        if (*pBuffer == delimiter || *pBuffer == 0) {
            if (!lastWasDelimiter) {
                result.push_back(mapped_buffer_t(pStart, pBuffer - pStart));
            }
            lastWasDelimiter = true;
        } else {
            if (lastWasDelimiter) {
                pStart = pBuffer;
            }
            lastWasDelimiter = false;
        }
    }
    if (!lastWasDelimiter) {
        result.push_back(mapped_buffer_t(pStart, pBuffer - pStart));
    }
    return result;
}

std::vector <mapped_buffer_t> buffer_split_on_first(mapped_buffer_t buffer, char delimiter) {
    char * pBuffer = (char *)buffer.buf;
    char * pStart = pBuffer;
    size_t bufferIndex;
    bool gotFirst = false;
    std::vector<mapped_buffer_t> result;

    for (bufferIndex = 0; bufferIndex < buffer.len && ! gotFirst; ++pBuffer, ++bufferIndex) {
        if (*pBuffer == delimiter) {
            result.push_back(mapped_buffer_t(pStart, pBuffer - pStart));
            // don't use break here, go back to the top and ensure that we have incremented pBuffer and bufferIndex
            // or else we fail to skip the delimiter in the second half of the result
            gotFirst = true;
        }
    }
    if (gotFirst) {
        result.push_back(mapped_buffer_t(pBuffer, buffer.len - bufferIndex));
    } else {
        result.push_back(buffer);
        result.push_back(mapped_buffer_t());
    }

    return result;
}

mapped_buffer_t & buffer_trim_front(mapped_buffer_t & buffer, char value) {
    if (buffer.len == 0) return buffer;

    size_t len = buffer.len;
    char * p = (char *) buffer.buf;

    if (value) {
        while (*p == value) {
            ++p;
            if (! --len) break;
        }
    } else {
        while (len && isspace(*p)) {
            ++p;
            if (! --len) break;
        }
    }
    buffer.buf = (len) ? p : 0;
    buffer.len = len;

    return buffer;
}

mapped_buffer_t & buffer_trim_back(mapped_buffer_t & buffer, char value) {

    if (buffer.len == 0) return buffer;

    size_t len = buffer.len;
    char * p = (char *) buffer.buf + len - 1;

    if (value) {
        while (len && *p == value) {
            --p;
            --len;
        }
    } else {
        while (len && isspace(*p)) {
            --p;
            --len;
        }
    }
    buffer.len = len;

    return buffer;
}

mapped_buffer_t & buffer_trim(mapped_buffer_t & buffer, char value) {

    buffer_trim_front(buffer, value);
    buffer_trim_back(buffer, value);

    return buffer;
}