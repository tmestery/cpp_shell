#include <stdio.h>
#include <stdbool.h>
#include "parse.h"

int main() {
    std::string input;
    while (true) {
        std::cout << "cpp_shell> ";
        std::getline(std::cin, input);
        parse(input);
    }
    return 0;
}