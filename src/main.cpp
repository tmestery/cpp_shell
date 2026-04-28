#include <stdio.h>
#include <stdbool.h>
#include "parse.h"
#include <signal.h>

int main() {
    // the shell now ignores Ctrl+C
    signal(SIGINT, SIG_IGN);

    // loop to collect shell commands
    std::string input;
    while (true) {
        std::cout << "cpp_shell> ";
        std::getline(std::cin, input);
        parse(input);
    }
    return 0;
}