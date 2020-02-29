//
// Created by Simon Willis on 26/05/2016.
//



#include "../src/components/utilities/buffers.h"

#include <string>

int main(int argc, char * argv[]) {

    int failed = 0;
    int expectedCount = 0;

    bool does;
    mapped_buffer_t buffer;

    buffer = mapped_buffer_t("ABCDEF");

    does = buffer_equals(buffer, "ABCDE");
    if (does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_equals. value shorter than buffer, returned true\n");
    }

    does = buffer_equals(buffer, "ABCDEFG");
    if (does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_equals. value long than buffer, returned true\n");
    }

    does = buffer_equals(buffer, "ABCDEF");
    if (!does) {
        ++failed;
        fprintf(stderr, "FAILED buffer_equals. values are equal, returned false\n");
    }

    does = buffer_starts_with(buffer, "AABCDE");
    if (does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_starts_with. Does not start with, returned true\n");
    }

    does = buffer_starts_with(buffer, "ABCDEFG");
    if (does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_starts_with. Too long to start with, returned true\n");
    }

    does = buffer_starts_with(buffer, "ABCDEF");
    if (!does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_starts_with. Values are equal so should be true, returned false\n");
    }

    does = buffer_starts_with(buffer, "ABCDE");
    if (!does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_starts_with. Does start with, returned false\n");
    }

    does = buffer_starts_with(buffer, "A");
    if (!does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_starts_with. Does start with, returned false\n");
    }

    does = buffer_ends_with(buffer, "F");
    if (!does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_ends_with. Does end with, returned false\n");
    }


    does = buffer_ends_with(buffer, "ABCDEF");
    if (!does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_ends_with. Does end with, returned false\n");
    }

    does = buffer_ends_with(buffer, "ABCDE");
    if (does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_ends_with. Does not end with, returned true\n");
    }

    does = buffer_ends_with(buffer, "abcdef", BUFFER_CASE_INSENSITIVE_CHECK);
    if (!does) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_ends_with. Does end with (case insentive), returned false\n");
    }


    buffer = mapped_buffer_t("ABCDEF");
    if (! buffer_contains(buffer, "AB")) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_contains did not spot contents at start of buffer\n");
    }
    if (! buffer_contains(buffer, "EF")) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_contains did not spot contents at end of buffer\n");
    }
    if (! buffer_contains(buffer, "CD")) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_contains did not spot contents at middle of buffer\n");
    }
    if (! buffer_contains(buffer, "ABCDEF")) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_contains did not spot contents when whole contents matched\n");
    }



    if (failed) return failed;

    //
    // from here on we can rely on the buffer comparitors
    //

    std::vector<mapped_buffer_t> result;

    char spaceDelimedStringWithLeadingAndTrailingDelimiter[] = " one two three ";
    buffer = mapped_buffer_t(spaceDelimedStringWithLeadingAndTrailingDelimiter,
                             sizeof(spaceDelimedStringWithLeadingAndTrailingDelimiter));

    result = buffer_split(buffer, ' ');

    if (result.size() != 3) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_split. returned wrong number of values  when there is a leading and trailing space, %ld instead of 3\n", result.size());
    } else {
        if (!buffer_equals(result[0], "one")) {
            ++failed;
            fprintf(stderr, "FAILED: buffer_split. first field returned incorrect when there is a leading space\n");
        }
        if (!buffer_equals(result[1], "two")) {
            ++failed;
            fprintf(stderr, "FAILED: buffer_split. middle field returned incorrect when there is a leading space\n");
        }
        if (!buffer_equals(result[2], "three")) {
            ++failed;
            fprintf(stderr, "FAILED: buffer_split. end field returned incorrect when there is a trailing space\n");
        }
    }

    char spaceDelimedStringWithoutLeadingAndTrailingDelimiter[] = "one two three";
    buffer = mapped_buffer_t(spaceDelimedStringWithoutLeadingAndTrailingDelimiter,
                             sizeof(spaceDelimedStringWithoutLeadingAndTrailingDelimiter));
    result = buffer_split(buffer, ' ');

    if (result.size() != 3) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_split. returned wrong number of values, %ld instead of 3\n", result.size());
    } else {
        if (!buffer_equals(result[0], "one")) {
            ++failed;
            fprintf(stderr, "FAILED: buffer_split. first field returned incorrect, got [%s] instead of [%s]\n", std::string(result[0].buf, result[0].len).c_str(), "one");
        }
        if (!buffer_equals(result[1], "two")) {
            ++failed;
            fprintf(stderr, "FAILED: buffer_split. middle field returned incorrect, got [%s] instead of [%s]\n", std::string(result[1].buf, result[1].len).c_str(), "two");
        }
        if (!buffer_equals(result[2], "three")) {
            ++failed;
            fprintf(stderr, "FAILED: buffer_split. end field returned incorrect, got [%s] [len=%ld] instead of [%s]\n", std::string(result[2].buf, result[2].len).c_str(), result[2].len, "three");
        }
    }

    char colonDelimedString[] = "A:B:C";

    result = buffer_split_on_first(colonDelimedString, ':');

    if (result.size() != 2) {
        ++failed;
        fprintf(stderr, "FAILED buffer_split_on_first. Delimited input. Returned wrong number of values, %ld instead of 2\n", result.size());
    } else {
        if (! buffer_equals(result[0], "A")) {
            ++failed;
            fprintf(stderr, "FAILED buffer_split_on_first. Delimited input. First result was [%s] expected[A]\n", std::string(result[0].buf, result[0].len).c_str());
        }
        if (! buffer_equals(result[1], "B:C")) {
            ++failed;
            fprintf(stderr, "FAILED buffer_split_on_first. Delimited input. Second result was [%s] expected[B:C]\n", std::string(result[1].buf, result[1].len).c_str());
        }
    }


    char nonColonDelimedString[] = "ABC";

    result = buffer_split_on_first(nonColonDelimedString, ':');

    if (result.size() != 2) {
        ++failed;
        fprintf(stderr, "FAILED buffer_split_on_first. Non-Demlimited input. Returned wrong number of values, %ld instead of 2\n", result.size());
    } else {
        if (! buffer_equals(result[0], "ABC")) {
            ++failed;
            fprintf(stderr, "FAILED buffer_split_on_first. Non-Demlimited input. First result was [%s] expected[ABC]\n", std::string(result[0].buf, result[0].len).c_str());
        }
    }

    buffer = mapped_buffer_t("  test");
    buffer_trim_front(buffer);
    if (std::string(buffer.buf, buffer.len).compare("test")) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_trim front (default) did not attain expected result, got len=%ld [%s]\n", buffer.len, std::string(buffer.buf, buffer.len).c_str());
    }

    buffer = mapped_buffer_t("\t\r\n\v\f  test");
    buffer_trim_front(buffer);
    if (std::string(buffer.buf, buffer.len).compare("test")) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_trim front (default) did not attain expected result, got len=%ld [%s]\n", buffer.len, std::string(buffer.buf, buffer.len).c_str());
    }


    buffer = mapped_buffer_t("----test");
    buffer_trim_front(buffer, '-');
    if (std::string(buffer.buf, buffer.len).compare("test")) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_trim front (default) did not attain expected result, got len=%ld [%s]\n", buffer.len, std::string(buffer.buf, buffer.len).c_str());
    }

    // Test with various line endings
    buffer = mapped_buffer_t(
            "11111\r\n" \
            "22222\r\n\r\n" \
            "33333\n" \
            "44444\r" \
            "55555\r\n"
    );
    expectedCount = 5;

    std::vector<mapped_buffer_t> lines = buffer_split_lines(buffer);
    if (lines.size() != expectedCount) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_split_lines did not retrieve correct number of lines, got [%ld] expected %d\n", lines.size(), expectedCount);
    } else {
        if (! buffer_equals(lines[0], "11111")) {
            fprintf(stderr, "FAILED: buffer_split_lines did not retrieve line 1 correctly, got [%s] expected [11111]\n", std::string(lines[0].buf, lines[0].len).c_str());
        }
        if (! buffer_equals(lines[1], "22222")) {
            fprintf(stderr, "FAILED: buffer_split_lines did not retrieve line 2 correctly, got [%s] expected [22222]\n", std::string(lines[1].buf, lines[1].len).c_str());
        }
        if (! buffer_equals(lines[2], "33333")) {
            fprintf(stderr, "FAILED: buffer_split_lines did not retrieve line 3 correctly, got [%s] expected [33333]\n", std::string(lines[2].buf, lines[2].len).c_str());
        }
    }

    // test where no trailing line end characters
    buffer = mapped_buffer_t(
            "11111\r\n" \
            "22222"
    );
    expectedCount = 2;

    lines = buffer_split_lines(buffer);
    if (lines.size() != expectedCount) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_split_lines did not retrieve correct number of lines, got [%ld] expected %d\n", lines.size(), expectedCount);
    } else {
        if (! buffer_equals(lines[0], "11111")) {
            fprintf(stderr, "FAILED: buffer_split_lines did not retrieve line 1 correctly, got [%s] expected [11111]\n", std::string(lines[0].buf, lines[0].len).c_str());
        }
        if (! buffer_equals(lines[1], "22222")) {
            fprintf(stderr, "FAILED: buffer_split_lines did not retrieve line 2 correctly, got [%s] expected [22222]\n", std::string(lines[1].buf, lines[1].len).c_str());
        }
    }


    // test where no line end characters
    buffer = mapped_buffer_t(
            "11111"
    );
    expectedCount = 1;

    lines = buffer_split_lines(buffer);
    if (lines.size() != expectedCount) {
        ++failed;
        fprintf(stderr, "FAILED: buffer_split_lines did not retrieve correct number of lines, got [%ld] expected %d\n", lines.size(), expectedCount);
    } else {
        if (! buffer_equals(lines[0], "11111")) {
            fprintf(stderr, "FAILED: buffer_split_lines did not retrieve line 1 correctly, got [%s] expected [11111]\n", std::string(lines[0].buf, lines[0].len).c_str());
        }
    }

    std::string str("1.0.0.0");
    buffer = mapped_buffer(str);
    std::vector<mapped_buffer> parts = buffer_split(buffer, '.');
    if (parts.size() != 4) {
        ++failed;
        fprintf(stderr, "FAILED: initialisation with std::string fails to produce a value\n");
    }




    if (! failed) {
        fprintf(stderr, "All tests passed\n");
    }
    return failed;

}