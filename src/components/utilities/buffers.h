//
// Created by Simon Willis on 24/02/2017.
//

#ifndef SRS_MAPPEDBUFFER_H
#define SRS_MAPPEDBUFFER_H


#include <vector>
#include <string>
#include <cstring> // for strlen - required by some compilers

#define BUFFER_CASE_INSENSITIVE_CHECK false
#define BUFFER_CASE_SENSITIVE_CHECK true

typedef struct mapped_buffer {
    mapped_buffer() : buf(0), len(0) { }
    mapped_buffer(std::string &buffer) : buf(buffer.data()), len(buffer.length()) { }
    mapped_buffer(const char *buffer) : buf(buffer), len(strlen(buffer)) { }
    mapped_buffer(const char *buffer, const size_t length) : buf(buffer), len(length) { }
    const std::string toString() { return std::string(buf, len); }
    const bool empty() { return (buf == 0 || len == 0); }

    const char *buf;
    size_t len;
} mapped_buffer_t;

std::vector <mapped_buffer_t> buffer_split(mapped_buffer_t buffer, char delimiter);
std::vector <mapped_buffer_t> buffer_split_on_first(mapped_buffer_t buffer, char delimiter);
std::vector <mapped_buffer_t> buffer_split_lines(mapped_buffer_t buffer);
bool buffer_equals(mapped_buffer_t buffer, const char * value, bool caseSensitivity = BUFFER_CASE_INSENSITIVE_CHECK);
bool buffer_equals(mapped_buffer_t buffer, mapped_buffer_t value, bool caseSensitivity = BUFFER_CASE_INSENSITIVE_CHECK);
bool buffer_starts_with(mapped_buffer_t buffer, const char * value, bool caseSensitivity = BUFFER_CASE_INSENSITIVE_CHECK);
bool buffer_ends_with(mapped_buffer_t buffer, const char * value, bool caseSensitivity = BUFFER_CASE_INSENSITIVE_CHECK);
bool buffer_contains(mapped_buffer_t buffer, const char * value, bool caseSensitivity = BUFFER_CASE_INSENSITIVE_CHECK);
std::string buffers_join(std::vector<mapped_buffer_t> &buffers, const char delimiter, uint8_t offset = 0);
mapped_buffer_t & buffer_trim_front(mapped_buffer_t & buffer, char value = 0);
mapped_buffer_t & buffer_trim_back(mapped_buffer_t & buffer, char value = 0);
mapped_buffer_t & buffer_trim(mapped_buffer_t & buffer, char value = 0);
long toLongInteger(mapped_buffer_t buffer, int radix = 10);
unsigned long toUnsignedLongInteger(mapped_buffer_t buffer, int radix = 10);
uint16_t toUnsignedShortInteger(mapped_buffer_t buffer, int radix = 10);
std::string bufferToString(mapped_buffer_t buffer);


#endif //SRS_MAPPEDBUFFER_H
