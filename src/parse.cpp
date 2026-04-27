#include <stdio.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include "parse.h"

void parse(std::string inputString) {
    if (inputString.empty()) return;

    std::istringstream lineStream(inputString);
    std::string command;
    lineStream >> command;

    if (command == "exit") {
        exit(0);
    } else if (command == "pwd") {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        std::cout << cwd << std::endl;
    } else if (command == "cd") {
        std::string path;
        lineStream >> path;
        if (chdir(path.c_str()) != 0) {
            perror("cd");
        }
    } else if (command == "echo") {
        std::string rest;
        std::getline(lineStream, rest);
        std::cout << rest << std::endl;
    } else if (command == "ls" || command == "cat" || command == "mkdir") {
        pid_t pid = fork();
        if (pid == 0) {
            // child process
            std::string arg;
            std::vector<std::string> args = {command};
            while (lineStream >> arg) {
                args.push_back(arg);
            }
            std::vector<char*> cargs;
            for (auto& a : args) {
                cargs.push_back(&a[0]);
            }
            cargs.push_back(nullptr);
            execvp(command.c_str(), cargs.data());
            perror("exec");
            exit(1);
        } else if (pid > 0) {
            // parent process
            wait(nullptr);
        } else {
            perror("fork");
        }
    } else {
        std::cout << command << ": command not found" << std::endl;
    }
}