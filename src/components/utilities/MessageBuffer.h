/// @file MessageBuffer.h
/// @brief Automated handling of delimited data. Developed to handle segmented TCP socket messages.
/// @author Simon Willis
/// @bug

#ifndef UTILITIES_MESSAGEBUFFER_H
#define UTILITIES_MESSAGEBUFFER_H

#include <string.h>
#include <vector>


class MessageBuffer {

public:

    /// @brief a message buffer that allows messages that may have become segmmented to be stored and then
    /// retrieved by specifying a message delimiter. TCP sockets often split up individual messages
    /// requiring them to be rebuilt based on basic rules. In the case of HTTP messages a typical
    /// message separator is '\r\n\r\n'. This is also true of SIP in that each main section of a message
    /// ends in '\r\n\r\n' whilst each line is delimited with '\r\n'. With SIP Messages that can contain
    /// multiple sub-messages the Content-Length property of the initial message is used to determine
    /// when the overall message has been received
    /// @param size the maximum number of bytes that can be stored at any one time, this does not resize
    /// @return
    MessageBuffer(size_t size);

    ~MessageBuffer();

    /// @brief loads data into the buffer. Attempts to load more than the buffer has space available are rejected
    /// @param buf a pointer to the data that is to be loaded
    /// @param len the number of bytes in the buffer to be loaded
    /// @return the number of bytes loaded. Zero if the available space was to handle the request
    size_t push(const char * buf, size_t len);

    /// @brief pull bytes out of the MessageBuffer, using a messageDelimiter if supplied
    /// @param buf a pointer to a location into which bufSize bytes can safely be copied
    /// @param bufSize the maximum nnumber of bytes that can be copied into the buffer
    /// @param delimiter a sequence of values that are used to determine the end of a message.
    /// Specifying or defaulting to the nullDelimiter will cause as many bytes that are available
    /// that can fit into the supplied buffer
    /// @return the number of bytes transfered
    size_t pull(char * buf, size_t bufSize, const std::vector<char> delimiter = nullDelimiter);

    /// @brief pull bytes out of the MessageBuffer, up to the number specified as available
    /// @param buf a pointer to a location into which bufSize bytes can safely be copied
    /// @param bufSize the maximum number of bytes that can be copied into the buffer
    /// @param bytes the number of bytes that are required
    /// @return the number of bytes transfered
    size_t pull(char * buf, size_t bufSize, const size_t bytes);

    /// @brief pull a character out of the buffer
    /// @return next character, or 0 if none available
    char getchar();

    /// @brief puts a character back into the start of buffer
    /// @return true if there was space to store the character
    bool putchar(char c);

    /// @brief get the number of bytes of storage available for additional message data to be stored
    /// @return max number of bytes of data that can be added with the next push
    size_t getSpaceAvailableByteCount();

    /// @brief get the number of bytes that are available to be pulled from the MessageBuffer
    /// @return number of bytes stored
    size_t getStoredByteCount();

    /// @brief clean out the MessageBuffer data
    /// @return
    size_t flush();

public:

    /// @brief a null delimiter is used where the data does not have delimiters or retrieval is byte limited
    static const std::vector<char> nullDelimiter;

private:
    size_t size;
    size_t maxCapacity;
    size_t dataStartPos;
    size_t dataEndPos;
    char * data = NULL;
};


#endif //UTILITIES_MESSAGEBUFFER_H
