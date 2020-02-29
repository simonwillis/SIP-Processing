//
// Created by simon on 17/09/16.
//

#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <string>
#include <sstream>
#include <vector>


bool areEquivalent(const char * s1, const char * s2, size_t max = 0);

void sleep(int seconds, int milliseconds = 0);

void printChar(char c);

template <typename Out>
void splitter(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &, char delim = ' ');

std::vector<std::string> split_on_first(const std::string &, char delim = ' ');


#endif //_UTIL_HPP
