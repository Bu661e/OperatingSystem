// my_shell.cpp
#include "Disk.h"
#include <iostream>
#include <sstream>

void executeCommand(Disk& disk, std::string commandLine) {
    std::istringstream iss(commandLine);
    std::string command, arg1, arg2;
    iss >> command >> arg1 >> arg2;

    if (command == "ls") {
        disk.ls();
    } else if (command == "cd") {
        disk.cd(arg1);
    } else if (command == "mv") {
        disk.mv(arg1, arg2);
    } else if (command == "touch") {
        disk.touch(arg1);
    } else if (command == "mkdir") {
        disk.mkdir(arg1);
    } else if (command == "rm") {
        disk.rm(arg1);
    } else if (command == "rmdir") {
        disk.rmdir(arg1);
    } else if (command == "read") {
        disk.read(arg1);
    } else if (command == "write") {
        disk.write(arg1, arg2);
    } else if (command == "exit") {
        disk.exit();
    } else {
        std::cout << "Unknown command" << std::endl;
    }
}

int main() {
    Disk disk;
    std::string commandLine;

    while (true) {
        std::cout << "/ ";
        std::getline(std::cin, commandLine);
        if (commandLine == "exit") {
            disk.exit();
            break;
        }
        executeCommand(disk, commandLine);
    }

    return 0;
}
