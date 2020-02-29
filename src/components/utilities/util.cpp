#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iterator>
#include <time.h>
#include "util.hpp"

bool areEquivalent(const char * s1, const char * s2, size_t max) {
    char * pS1 = (char *)s1;
    char * pS2 = (char *)s2;
    size_t processing = max;
    while (--processing && *pS1 && *pS2) {
        if (toupper((*pS1++)) != toupper((*pS2++))) return false;
    }
    return true;
}

void printChar(char c) {
    if (isprint(c))
        fprintf(stderr, "%c", c);
    else if (c == '\r')
        fprintf(stderr, "\\r");
    else if (c == '\n')
        fprintf(stderr, "\\n\n");
    else
        fprintf(stderr ,"(%d)", c);
}

void sleep(int seconds, int milliseconds) {
    struct timespec ts;
    ts.tv_sec = seconds;
    ts.tv_nsec = milliseconds * 1000000;
    nanosleep(&ts, NULL);
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss;
    std::vector<std::string> elems;
    splitter(s, delim, std::back_inserter(elems));
    return elems;
}

template <typename Out>
void splitter(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split_on_first(const std::string &s, char delim) {
    std::vector<std::string> elems;
    size_t pos = s.find(delim);
    if (pos != std::string::npos) {
        elems.push_back(s.substr(0, pos));
        elems.push_back(s.substr(pos + 1));
    } else {
        elems.push_back(s);
        elems.push_back(std::string());
    }
    return elems;
}