#include <stdio.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include "parse.h"

void handlePiping(std::string left, std::string right) {
    int fd[2];
    pipe(fd);  // fd[0] = read end, fd[1] = write end

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // left command
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        parse(left);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // right command
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        parse(right);
        exit(0);
    }

    // parent closes both + waits
    close(fd[0]);
    close(fd[1]);
    wait(nullptr);
    wait(nullptr);
}

void parse(std::string inputString) {
    if (inputString.empty()) return;

    // Remote whitespace
    inputString.erase(0, inputString.find_first_not_of(" \t\n"));
    inputString.erase(inputString.find_last_not_of(" \t\n") + 1);

    // check for pipe
    size_t pipePos = inputString.find('|');
    if (pipePos != std::string::npos) {
        std::string left = inputString.substr(0, pipePos);
        std::string right = inputString.substr(pipePos + 1);
        handlePiping(left, right);
        return;
    }

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
    } else {
    // fork/exec anything
    pid_t pid = fork();
    if (pid == 0) {
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
        wait(nullptr);
    } else {
        perror("fork");
    }
}
}