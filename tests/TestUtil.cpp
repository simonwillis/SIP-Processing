//
// Created by simon on 03/04/17.
//


#include "../src/components/utilities/util.hpp"


bool testSplit() {
    int errorCount = 0;

    std::string input = "one,";
    std::vector<std::string> output;

    input = std::string("one,");
    output = split(input, ',');

    input = std::string("one,two, three");
    output = split(input, ',');

    input = std::string("one,");
    output = split(input, ',');

    return (errorCount > 0);
}

int main(int argc, char * argv[]) {
    int errorCount = 0;

    errorCount += testSplit();

}
