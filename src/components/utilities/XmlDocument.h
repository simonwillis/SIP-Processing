//
// Created by Simon Willis on 25/02/2017.
//

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <cstring>
#include <vector>
#include <unordered_map>
#include <string>
#include "XmlDocument.h"

class XmlDocument {

private:
    struct property {
        bool valid;
        std::string name;
        std::string value;
    };
    struct node {
        bool valid;
        std::string name;
        std::string value;
        uint16_t depth;
        std::unordered_map<std::string, std::string> properties;
        std::vector<node> nodes;
    };

public:
    const size_t MaxPathLength = 2048;

public:
    XmlDocument(const char * buf, const size_t len);
    ~XmlDocument() {};

    bool isValid() { return valid; }

    std::string getValue(std::string path, std::string defaultValue = "");
    std::unordered_map<std::string, std::string> getProperties(std::string path);

private:
    XmlDocument() : dataSize(0), pDataEnd(0) {};

private:

    char * read(bool skipSpaces = false);
    char lookAhead(bool skipSpaces = false);
    //char lookAhead(int count);
    void skipWhiteSpace();
    char * unread();
    char * pBuf;

    bool parse();

    XmlDocument::node parseNode(uint16_t depth);
    std::unordered_map<std::string, std::string> parseProperties(char termChar);
    XmlDocument::property parseProperty(char termChar);
    std::string readValue(char termChar, bool allowSpaces);

    void printNode(XmlDocument::node node);

private:
    const size_t dataSize;
    const char * pDataStart;
    const char * pDataEnd;
    bool valid = false;
    XmlDocument::node root;
};

#endif //XMLPARSER_H
