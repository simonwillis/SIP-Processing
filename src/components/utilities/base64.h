#ifndef UTILITY_BASE64_H
#define UTILITY_BASE64_H

#include <vector>
#include <string>
//#include <sys/dtrace.h>

typedef unsigned char byte_t;

std::string base64Encode(byte_t const* buf, unsigned int bufLen);

std::vector<byte_t> base64Decode(std::string const&);

#endif //UTILITY_BASE64_H
