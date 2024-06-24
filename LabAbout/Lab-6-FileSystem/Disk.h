// Disk.h
#ifndef DISK_H
#define DISK_H

#include "File.h"
#include <string>
#include <vector>

class Disk {
private:
    File* root;
    File* currentDirectory;

    File* findFile(std::string path);

public:
    Disk();
    ~Disk();

    void ls();
    void cd(std::string dirName);
    void mv(std::string srcName, std::string destName);
    void touch(std::string fileName);
    void mkdir(std::string dirName);
    void rm(std::string fileName);
    void rmdir(std::string dirName);
    void read(std::string fileName);
    void write(std::string fileName, std::string content);
    void exit();
};

#endif // DISK_H
