//
// Created by Simon Willis on 07/07/2016.
//

#include "MessageBuffer.h"

#include <cstring> // required for memcpy with some compilers, ignore the unused notification
#include <cstdio>

const std::vector<char> MessageBuffer::nullDelimiter = std::vector<char>{};

MessageBuffer::MessageBuffer(size_t capacity)
        : size(capacity + 1), maxCapacity(capacity), dataStartPos(0), dataEndPos(0) {

    if (capacity) {
        data = new char[size];
        //memset(data, 0, size);
    }
}

MessageBuffer::~MessageBuffer() {
    if (data) {
        delete data;
    }
}

size_t MessageBuffer::push(const char * buf, size_t len) {
    size_t bytesPushed = 0;
    size_t spaceAvailableByteCount = getSpaceAvailableByteCount();

    if (len > spaceAvailableByteCount) {
        return bytesPushed;
    }

    size_t count = size - dataEndPos;
    count = count < len ? count : len;
    memcpy((void*)(data + dataEndPos), (void *)buf, count);

    dataEndPos += count;

    if (count < len) {
        memcpy((void *)data, (void *)(buf + count), len - count);
        dataEndPos = len - count;
    }

    return len;
}

char MessageBuffer::getchar() {
    char c = 0;
    if (getStoredByteCount()) {
        c = *(data + dataStartPos++);
        if (dataStartPos >= size) dataStartPos = 0;
    }
    return c;
}

bool MessageBuffer::putchar(char c) {
    bool space = getSpaceAvailableByteCount();
    if (space) {
        --dataStartPos;
        if (dataStartPos >= size) dataStartPos = size;
        *(data + dataStartPos) = c;
    }
    return space;
}

size_t MessageBuffer::pull(char * buf, size_t bufSize, size_t bytes) {

    size_t storedByteCount = getStoredByteCount();
    size_t count;

    memset(buf, 0, bufSize);

    // don't attempt to pull more than we have
    count = bytes < storedByteCount ? bytes : storedByteCount;

    // Now copy the data into the buffer
    if (count > (size - dataStartPos)) {
        // we overlap the end and need to get some from the end and the rest from the start
        size_t endCount = size - dataStartPos;
        size_t startCount = count - endCount;
        memcpy((void *)buf, (void *)(data + dataStartPos), endCount);
        memcpy((void *)(buf + endCount), (void *)data, startCount);
        dataStartPos = startCount;
    } else if (count) {
        // this is a straight collect and update
        memcpy((void *)buf, (void *)(data + dataStartPos), count);
        dataStartPos += count;
    }

    return count;
}

size_t MessageBuffer::pull(char * buf, size_t bufSize, const std::vector<char> delimiter) {

    size_t delimiterLength = delimiter.size();
    size_t storedByteCount = getStoredByteCount();
    size_t count;

    if (delimiterLength > 0) {

        if (storedByteCount < delimiterLength) {
            // never to find it so don't bother
            return 0;
        }

        // we need to skip along looking for the delimiter

        char * pSource = data + dataStartPos;
        char * pEnd = data + size;
        size_t countToMatch = delimiterLength;
        size_t matchedCount = 0;
        bool haveMatch = false;

        count = 0;

        do {
            if (matchedCount == countToMatch) {
                haveMatch = true;
                break;
            }

            // make sure we skip back to the start of the buffer
            if (pSource == pEnd) {
                pSource = data;
            }

            if (*pSource == delimiter[matchedCount]) {
                ++matchedCount;
            } else {
                matchedCount = 0;
            }

            ++pSource;
            ++count;

            if (count >= bufSize) {
                break;
            }
        } while (true);

        if (!haveMatch) {
            count = 0;
        }

    } else {
        // pull as much as we can
        count = bufSize < storedByteCount ? bufSize : storedByteCount;
    }

    count = pull(buf, bufSize, count);

    return count;
}

size_t MessageBuffer::flush() {
    dataStartPos = 0;
    dataEndPos = 0;
    return maxCapacity;
}

size_t MessageBuffer::getStoredByteCount() {
    if (dataEndPos >= dataStartPos) {
        return dataEndPos - dataStartPos;
    }
    return (size - dataStartPos) + dataEndPos;
}

size_t MessageBuffer::getSpaceAvailableByteCount() {
    return maxCapacity - getStoredByteCount();
}
