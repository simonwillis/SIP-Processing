//
// Created by Simon Willis on 25/02/2017.
//

#include <cstdlib>
#include "XmlDocument.h"
#include "util.hpp"


XmlDocument::XmlDocument(const char *buf, size_t len)
    : dataSize(len), pDataStart(buf), pDataEnd(buf + len) {

    pBuf = (char *)pDataStart;
    root.depth = 0;

    //fprintf(stderr, "XmlDocument::CTOR data length is %zu\n", dataSize);

    parse();
}

std::string XmlDocument::getValue(std::string path, std::string defaultValue) {
    std::string result = defaultValue;
    node * node = &root;
    std::vector<std::string> pathElements = split(path, '.');
    size_t elementCount = pathElements.size();
    bool foundSubnode;
    bool isLast;

    for (size_t e = 0; e < elementCount; ++e) {
        isLast = (e == elementCount - 1);
        std::string elementName = pathElements[e];

        if (isLast) {
            const std::unordered_map<std::string, std::string>::const_iterator located = node->properties.find(elementName);
            if (located != node->properties.end()) {
                result = located->second;
            } else {
                for (std::vector<XmlDocument::node>::iterator n = node->nodes.begin(); n != node->nodes.end(); ++n) {
                    if (! (*n).name.compare(elementName)) {
                        node = &(*n);
                        result = node->value;
                        break;
                    }
                }
            }
            break;
        }

        foundSubnode = false;

        for (std::vector<XmlDocument::node>::iterator n = node->nodes.begin(); n != node->nodes.end(); ++n) {
            if (! (*n).name.compare(elementName)) {
                node = &(*n);
                foundSubnode = true;
                break;
            }
        }
        if (! foundSubnode) {
            fprintf(stderr, "Failed to locate subnode '%s' at index %zu\n", elementName.c_str(), e);
            break;
        }
    }
    return result;
}

std::unordered_map<std::string, std::string> XmlDocument::getProperties(std::string path) {
    std::unordered_map<std::string, std::string> result;

    node * node = &root;
    std::vector<std::string> pathElements = split(path, '.');
    size_t elementCount = pathElements.size();
    bool foundSubnode;
    bool isLast;

    for (size_t e = 0; e < elementCount; ++e) {
        isLast = (e == elementCount - 1);
        std::string elementName = pathElements[e];

        for (std::vector<XmlDocument::node>::iterator n = node->nodes.begin(); n != node->nodes.end(); ++n) {
            if (! (*n).name.compare(elementName)) {
                node = &(*n);
                foundSubnode = true;
                break;
            }
        }
        if (! foundSubnode) {
//            fprintf(stderr, "Failed to locate subnode '%s' at index %d\n", elementName.c_str(), e);
            break;
        }
        if (isLast) {
            result = node->properties;
            for (auto it = node->nodes.begin(); it != node->nodes.end(); ++it) {
                if (! it->value.empty()) {
                    std::string name = std::string(it->name);
                    std::string value = std::string(it->value);
                    std::pair<std::string, std::string> pair = std::make_pair(name, value);
                    result.insert(pair);
                }
            }
        }
    }
    return result;
};

void XmlDocument::printNode(XmlDocument::node node) {
    uint32_t size;
    uint32_t index;
    std::string padding = std::string(node.depth * 4, ' ');
    fprintf(stderr, "%s%s", padding.c_str(), node.name.c_str());
    if (! node.value.empty()) {
        fprintf(stderr, " -> %s\n", node.value.c_str());
    } else {
        fprintf(stderr, "\n");
    }
    size = node.properties.size();
    for (auto it = node.properties.begin(); it != node.properties.end(); ++it) {
        fprintf(stderr, "%s    %s=%s\n", padding.c_str(), it->first.c_str(), it->second.c_str());
    }
    size = node.nodes.size();
    for (index = 0; index < size; ++index) {
        printNode(node.nodes[index]);
    }

}

char * XmlDocument::read(bool skipSpaces) {
    char * p = 0;
    while ((pBuf <= pDataEnd) && ((*pBuf == 10 || *pBuf == 13) || (skipSpaces && (*pBuf == ' ')))) ++pBuf;
    if (pBuf <= pDataEnd) {
        p = pBuf++;
    }
    return p;
}

char XmlDocument::lookAhead(bool skipSpaces) {
    char c = 0;
    char * p = pBuf;
    while ((p <= pDataEnd) && ((*p == 10 || *p == 13) || (skipSpaces && (*p == ' ')))) ++p;
    if (p <= pDataEnd) {
        c = *p;
    }
    //fprintf(stderr, "XmlDocument::lookAhead(%c) returning %c [0x%02x] offset=%ld\n", skipSpaces ? 'T':'F', c, c, p - pDataStart);
    return c;
}

//char XmlDocument::lookAhead(int count) {
//    char c = 0;
//    char * p = pBuf + count - 1; // -1 because the pointer is already on the next item to read
//    if (p >= pDataStart && p <= pDataEnd) {
//        c = *p;
//    }
//    return c;
//}

void XmlDocument::skipWhiteSpace() {
    if (isspace(*pBuf)) {
        while ((pBuf <= pDataEnd) && (*pBuf == 10 || *pBuf == 13 || isspace(*pBuf))) ++pBuf;
        --pBuf;
    }
}

char * XmlDocument::unread() {
    char *p = 0;
    while ((pBuf > pDataStart) && (*pBuf == 10 || *pBuf == 13)) --pBuf;
    if (pBuf > pDataStart) {
        p = --pBuf;
    } else {
        p = 0;
    }
    return p;
}

std::string XmlDocument::readValue(char termChar, bool allowSpaces) {
    char * p = 0;
    char * pStart = 0;
    char * pEnd = 0;
    bool done = false;
    bool isEscaped = false;
    bool isQuoted = false;

    skipWhiteSpace();

    while (! done && (p = read())) {

        if (! pStart) pStart = p;

        if (*p == '>' || *p == '<') {
            unread();
            done = true;
            pEnd = pBuf;
        } else if (*p == termChar && lookAhead() == '>') {
            done = true;
            pEnd = pBuf - 1;
        } else if (*p == '\\') {
            isEscaped = ! isEscaped;
        } else if (*p == '\"') {
            isQuoted = ! isQuoted;
        } else if (isspace(*p)) {
            if (! isQuoted && ! allowSpaces) {
                unread();
                done = true;
                pEnd = pBuf;
            }
        }
    }

    return std::string(pStart, pEnd - pStart);
}

XmlDocument::node XmlDocument::parseNode(uint16_t depth) {

    XmlDocument::node node;
    node.depth = depth;
    node.valid = false;

    char termChar = '/';
    char * p;

    //fprintf(stderr, "XmlDocument::parseNode [depth=%u]\n", depth);

    skipWhiteSpace();

    p = read(true);

    if (*p != '<') {
        //fprintf(stderr, "XmlDocument::parseNode Expected '<' but got '%c' at start of node\n", *p);
        return node;
    }

    if (lookAhead() == '?') {
        termChar = '?'; // special case of xml header line
        read(); // skip the character
        //fprintf(stderr, "XmlDocument::parseNode xml header line detected\n");
    } else if (lookAhead() != '/') {
        //fprintf(stderr, "XmlDocument::parseNode head of new node detected\n");
    } else {
        unread();
        //fprintf(stderr, "XmlDocument::parseNode node terminator located, aborting\n");
        return node;
    }

    node.name = readValue(termChar, false);

    // properties are kept at front of node after name, they are optional
    node.properties = parseProperties(termChar);

    // if the node completes with a termChar then it is a simple node and can be returned, otherwise there is content
    skipWhiteSpace();

    p = read();
    if (*p == termChar && lookAhead() == '>') {
        //fprintf(stderr, "XmlDocument::parseNode termChar picked up\n");
        p = read();
        if (*p == '>') {
            node.valid = true;
        } else {
            node.valid = false;
            fprintf(stderr, "XmlDocument::parseNode failed to pick up end of simple node item\n");
        }
    } else if (*p == '>') {
        //fprintf(stderr, "XmlDocument::parseNode looking for node content\n");
        skipWhiteSpace();
        if (lookAhead() == '<') {
            // expecting one or more nodes and possibly subnodes
            //fprintf(stderr, "XmlDocument::parseNode looking for Subnodes\n");
            XmlDocument::node subNode;
            do {
                //fprintf(stderr, "XmlDocument::parseNode %s depth %d calling parseNode\n", bufferToString(node.name).c_str(), depth);
                subNode = parseNode(depth + 1);
                if (subNode.valid) {
                    //fprintf(stderr, "XmlDocument::parseNode depth %d received %s subnode of depth %d, name %s\n", depth, subNode.valid ? "valid":"invalid",  subNode.depth, bufferToString(subNode.name).c_str());
                    node.nodes.push_back(subNode);
                }
            } while (subNode.valid);

        } else {
            //fprintf(stderr, "XmlDocument::parseNode looking for node content\n");
            // expecting body content, i.e. a string
            node.value = readValue(0, true);
        }

        //fprintf(stderr, "XmlDocument::parseNode %s looking for tail\n", bufferToString(node.name).c_str());

        // expecting tail of node now </name>

        if ((*(read()) == '<') && (*(read()) == '/')) {
            std::string endTag = readValue(0, false);
            if (! node.name.compare(endTag)) {
                node.valid = true;
                //fprintf(stderr, "XmlDocument::parseNode matched tail tag of node with name of node '%s'\n", bufferToString(endTag).c_str());
            } else {
                fprintf(stderr, "XmlDocument::parseNode failed to match node name '%s' with end tag '%s'\n", node.name.c_str(), endTag.c_str());
                node.valid = false;
            }
        } else {
            fprintf(stderr, "XmlDocument::parseNode unexpected node tail entry\n");
            node.valid = false;
        }
        read(); // consume final brace
    }
    if (! node.valid) {
        fprintf(stderr, "XmlDocument::parseNode returning Node [valid=%s] [name=%s]\n", node.valid ? "true" : "false", node.name.c_str());
    }
    //printNode(node);
    return node;

}

std::unordered_map<std::string, std::string> XmlDocument::parseProperties(char termChar) {
    std::unordered_map<std::string, std::string> properties;

    //fprintf(stderr, "XmlDocument::parseProerties\n");

    XmlDocument::property property;

    do {
        property = parseProperty(termChar);
        if (property.valid) {
            properties.insert(std::make_pair(property.name, property.value));
        }
    } while (property.valid);

    //fprintf(stderr, "parseProperties returning %d properties\n", properties.size());

    return properties;
}

XmlDocument::property XmlDocument::parseProperty(char termChar) {
    XmlDocument::property property;
    property.valid = false;
    char * pStart;
    char * pEnd;
    bool quoted = false;
    bool escaped = false;
    bool done = false;
    char * p;

    //fprintf(stderr, "XmlDocument::parseProperty at offset %ld\n", pBuf - pDataStart);

    property.valid = false;

    p = read(true);

    if (*p == '>' || (*p == termChar && lookAhead() == '>')) {
        //fprintf(stderr, "XmlDocument::parseProperty no properties found\n");
        unread(); // the node parser needs to see the closing characters
        return property;
    }

    pStart = p;

    while (! done) {
        p = read();
        if (!p) break;
        if ((*p == '>') || (*p == termChar && lookAhead() == '>')) {
            //fprintf(stderr, "XmlDocument::parseProperty at end of properties\n");
            pEnd = pBuf - 1;
            unread(); // leave the termination char(s) for the node parser to use
            done = true;
        } else if (*p == '\\') {
            escaped = !escaped;
            //fprintf(stderr, "XmlDocument::parseProperty escaped=%s\n", escaped ? "true":"false");
        } else if (*p == '\"') {
            if (! escaped) {
                quoted = !quoted;
                //fprintf(stderr, "XmlDocument::parseProperty quoted=%s\n", quoted ? "true":"false");
            }
        } else if (isspace(*p)) {
            if (! quoted) {
                //fprintf(stderr, "XmlDocument::parseProperty unquoted space found\n");
                pEnd = p;
                done = true;
            }
        }
    }
    std::string kvp = std::string(pStart, pEnd - pStart);
    std::vector<std::string> content = split_on_first(kvp, '=');
    if (content.size() == 2) {
        if (content[1].length()) {
            int front = 0, back = 0;
            if (content[1].front() == '\"') ++front;
            if (content[1].back() == '\"') ++back;
            if (front + back) {
                content[1] = content[1].substr(front, content[1].size() - (front + back));
            }
        }

        property.name = content[0];
        property.value = content[1];
        property.valid = true;
    }

    if (! property.valid) {
        fprintf(stderr, "parseProperty returning [valid=%s] [name=%s] [value=%s]\n", property.valid ? "true" : "false",
                property.name.c_str(), property.value.c_str());
    }

    return property;
}

bool XmlDocument::parse() {
    bool done = false;

    //fprintf(stderr, "XmlDocument::parse\n");

    //pNode = &root;

    do {
        //fprintf(stderr, "XmlDocument::parser calling parseNode\n");
        XmlDocument::node node = parseNode(0);
        if (node.valid) {
            root.nodes.push_back(node);
        } else {
            //fprintf(stderr, "XmlDocument::parse invalid node returned [name=%s]\n", node.name.bufferToString().c_str());
            done = true;
        }

    } while (! done);

    printNode(root);

    if (root.nodes.size() > 0) {
        valid = true;
    }


    return true;
}
