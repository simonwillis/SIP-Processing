//
// Created by Simon Willis on 07/07/2016.
//

#include <string>

#include "../src/components/utilities/MessageBuffer.h"


int main(int argc, char * argv[]) {
    size_t size;
    std::string string24("012345678901234567890123");
    std::string string16("0123456789ABCDEF");
    std::string string8("01234567");
    std::string string7("0123456");
    std::string stringTerminatedABCDEF("ABCDEF\r\n\r\n");
    std::string stringTerminated012345("012345\r\n\r\n");
    std::string string012345crlfcrlf012345crlfcrlf("012345\r\n\r\n012345\r\n\r\n");
    std::vector<char> delimiter = {'\r', '\n', '\r', '\n'};

    MessageBuffer mb = MessageBuffer(24);

    char largeBuffer[1024];
    char smallBuffer[16];
    char c;


    size = mb.getStoredByteCount();

    fprintf(stderr, "Stored ByteCount in buffer is %zu\n", size);

    size = mb.getSpaceAvailableByteCount();

    fprintf(stderr, "Space Available ByteCount in empty buffer is %zu\n", size);

    size = mb.push(string16.c_str(), string16.length());

    fprintf(stderr, "push of %ld bytes returned %zu\n", string16.length(), size);

    size = mb.push(string8.c_str(), string8.length());

    fprintf(stderr, "push of %ld bytes returned %zu\n", string8.length(), size);

    size = mb.getStoredByteCount();

    fprintf(stderr, "Stored ByteCount in buffer is %zu\n", size);

    size = mb.push(string7.c_str(), string7.length());

    fprintf(stderr, "push of %ld bytes returned %zu\n", string7.length(), size);

    size = mb.getStoredByteCount();

    fprintf(stderr, "Stored ByteCount in buffer is %zu\n", size);

    size = mb.getSpaceAvailableByteCount();

    fprintf(stderr, "Space Available ByteCount in buffer is %zu\n", size);

    size = mb.flush();

    fprintf(stderr, "Flush Buffer returned %zu\n", size);

    size = mb.getSpaceAvailableByteCount();

    fprintf(stderr, "Space Available ByteCount in buffer is %zu\n", size);

    size = mb.push(stringTerminatedABCDEF.c_str(), stringTerminatedABCDEF.length());

    fprintf(stderr, "push of %ld bytes returned %zu\n", stringTerminatedABCDEF.length(), size);

    size = mb.pull(largeBuffer, sizeof(largeBuffer), delimiter);

    fprintf(stderr, "\n\npull returned %zu [%s]\n\n", size, largeBuffer);

    size = mb.flush();

    fprintf(stderr, "Flush Buffer returned %zu\n", size);

    size = mb.getSpaceAvailableByteCount();

    fprintf(stderr, "Space Available ByteCount in buffer is %zu\n", size);

    size = mb.push(stringTerminatedABCDEF.c_str(), stringTerminatedABCDEF.length());

    fprintf(stderr, "push of %ld bytes returned %zu\n", stringTerminatedABCDEF.length(), size);

    size = mb.push(stringTerminated012345.c_str(), stringTerminated012345.length());

    fprintf(stderr, "push of %ld bytes returned %zu\n", stringTerminated012345.length(), size);

    size = mb.pull(largeBuffer, sizeof(largeBuffer), delimiter);

    fprintf(stderr, "\n\npull returned %zu [%s]\n\n", size, largeBuffer);

    size = mb.pull(largeBuffer, sizeof(largeBuffer), delimiter);

    fprintf(stderr, "\n\npull returned %zu [%s]\n\n", size, largeBuffer);

    size = mb.getSpaceAvailableByteCount();

    fprintf(stderr, "Space Available ByteCount in buffer is %zu\n", size);

    size = mb.pull(largeBuffer, sizeof(largeBuffer), delimiter);

    fprintf(stderr, "\n\npull returned %zu [%s]\n\n", size, largeBuffer);

    size = mb.push(stringTerminatedABCDEF.c_str(), stringTerminatedABCDEF.length());

    fprintf(stderr, "push of %ld bytes returned %zu\n", stringTerminatedABCDEF.length(), size);

    size = mb.pull(largeBuffer, sizeof(largeBuffer), delimiter);

    fprintf(stderr, "\n\npull returned %zu [%s]\n\n", size, largeBuffer);

    size = mb.getSpaceAvailableByteCount();

    fprintf(stderr, "Space Available ByteCount in buffer is %zu\n", size);

    size = mb.getStoredByteCount();

    fprintf(stderr, "Stored ByteCount in buffer is %zu\n", size);

    size = mb.flush();

    fprintf(stderr, "Flush Buffer returned %zu\n", size);

    size = mb.push(string24.c_str(), string24.length());

    fprintf(stderr, "push of %ld bytes returned %zu\n", string24.length(), size);

    memset(largeBuffer, 0, sizeof(largeBuffer));
    size = mb.pull(largeBuffer, sizeof(largeBuffer));

    fprintf(stderr, "\n\npull returned %zu [%s]\n\n", size, largeBuffer);

    size = mb.push(string7.c_str(), string7.length());

    fprintf(stderr, "push of %ld bytes returned %zu\n", string7.length(), size);

    memset(largeBuffer, 0, sizeof(largeBuffer));
    size = mb.pull(largeBuffer, sizeof(largeBuffer));

    fprintf(stderr, "\n\npull returned %zu [%s]\n\n", size, largeBuffer);

    mb.flush();
    size = mb.push(string012345crlfcrlf012345crlfcrlf.c_str(), string012345crlfcrlf012345crlfcrlf.length());
    fprintf(stderr, "push of %s (%ld bytes) returned %zu\n", string012345crlfcrlf012345crlfcrlf.c_str(), string012345crlfcrlf012345crlfcrlf.length(), size);
    size = mb.pull(largeBuffer, sizeof(largeBuffer), delimiter);
    fprintf(stderr, "\n\npull returned %zu [%s]\n\n", size, largeBuffer);
    size = mb.pull(largeBuffer, sizeof(largeBuffer), delimiter);
    fprintf(stderr, "\n\npull returned %zu [%s]\n\n", size, largeBuffer);


    mb.flush();
    mb.push("++++++++++", 10);
    mb.putchar('0');
    mb.putchar('1');
    mb.putchar('2');
    mb.putchar('3');
    mb.putchar('4');
    mb.putchar('5');
    mb.putchar('6');
    mb.putchar('7');
    mb.putchar('8');
    mb.putchar('9');
    mb.putchar('A');
    mb.putchar('B');
    mb.putchar('C');
    mb.putchar('D');
    mb.putchar('E');
    mb.putchar('F');
    while ((c = mb.getchar())) {
        fprintf(stderr, "%c (%d)", c, c);
    }
    fprintf(stderr, "\n");

    mb.putchar('Z');
    while ((c = mb.getchar())) {
        fprintf(stderr, "%c (%d)", c, c);
    }
    fprintf(stderr, "\n");

    mb.flush();
    mb.push("abcdefghijklmnopqrstuvwx", 24);
    while ((c = mb.getchar())) {
        fprintf(stderr, "%c (%d)", c, c);
    }
    fprintf(stderr, "\n");

    mb.flush();
    mb.push("abcdefghijklmnopqrstuvwx", 24);

    mb.putchar(mb.getchar());

    while ((c = mb.getchar())) {
        fprintf(stderr, "%c (%d)", c, c);
    }
    fprintf(stderr, "\n");

    mb.flush();
    mb.push("12345678901234567890", 20);
    mb.pull(largeBuffer, sizeof(smallBuffer), 20);

    mb.putchar('0');
    mb.putchar('1');
    mb.putchar('2');
    mb.putchar('3');
    mb.putchar('4');
    mb.putchar('5');
    mb.putchar('6');
    mb.putchar('7');
    mb.putchar('8');
    mb.putchar('9');
    mb.putchar('A');
    mb.putchar('B');
    mb.putchar('C');
    mb.putchar('D');
    mb.putchar('E');
    mb.putchar('F');

    memset(largeBuffer, 0, sizeof(largeBuffer));
    mb.pull(largeBuffer, sizeof(smallBuffer), 16);

    fprintf(stderr, "%s\n", largeBuffer);

    return 0;
}