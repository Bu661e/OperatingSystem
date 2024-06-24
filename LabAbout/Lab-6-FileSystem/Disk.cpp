// Disk.cpp
#include "Disk.h"
#include <iostream>
#include <iomanip>

Disk::Disk() {
    root = new File("/", true);
    currentDirectory = root;
}

Disk::~Disk() {
    delete root;
}

File* Disk::findFile(std::string path) {
    // Implement path parsing and file finding logic
    return nullptr;
}

void Disk::ls() {
    size_t totalSize = 0;
    for (const auto& child : currentDirectory->children) {
        totalSize += child.second->size;
    }

    std::cout << "total: " << totalSize << std::endl;
    std::cout << std::left << std::setw(10) << "name" 
              << std::setw(10) << "type" 
              << std::setw(10) << "size" 
              << std::setw(20) << "FCB" 
              << std::setw(15) << "dataStartBlock" << std::endl;

    for (const auto& child : currentDirectory->children) {
        std::cout << std::left << std::setw(10) << child.second->name 
                  << std::setw(10) << (child.second->isDirectory ? "directory" : "file")
                  << std::setw(10) << child.second->size
                  << std::setw(20) << child.second->FCB
                  << std::setw(15) << child.second->dataStartBlock << std::endl;
    }
}

void Disk::cd(std::string dirName) {
    if (currentDirectory->children.find(dirName) != currentDirectory->children.end() &&
        currentDirectory->children[dirName]->isDirectory) {
        currentDirectory = currentDirectory->children[dirName];
    } else {
        std::cout << "Directory not found" << std::endl;
    }
}

void Disk::mv(std::string srcName, std::string destName) {
    if (currentDirectory->children.find(srcName) != currentDirectory->children.end()) {
        File* file = currentDirectory->children[srcName];
        currentDirectory->children.erase(srcName);
        file->name = destName;
        currentDirectory->children[destName] = file;
    } else {
        std::cout << "File not found" << std::endl;
    }
}

void Disk::touch(std::string fileName) {
    if (currentDirectory->children.find(fileName) == currentDirectory->children.end()) {
        currentDirectory->children[fileName] = new File(fileName, false);
    } else {
        std::cout << "File already exists" << std::endl;
    }
}

void Disk::mkdir(std::string dirName) {
    if (currentDirectory->children.find(dirName) == currentDirectory->children.end()) {
        currentDirectory->children[dirName] = new File(dirName, true);
    } else {
        std::cout << "Directory already exists" << std::endl;
    }
}

void Disk::rm(std::string fileName) {
    if (currentDirectory->children.find(fileName) != currentDirectory->children.end() &&
        !currentDirectory->children[fileName]->isDirectory) {
        delete currentDirectory->children[fileName];
        currentDirectory->children.erase(fileName);
    } else {
        std::cout << "File not found" << std::endl;
    }
}

void Disk::rmdir(std::string dirName) {
    if (currentDirectory->children.find(dirName) != currentDirectory->children.end() &&
        currentDirectory->children[dirName]->isDirectory) {
        delete currentDirectory->children[dirName];
        currentDirectory->children.erase(dirName);
    } else {
        std::cout << "Directory not found" << std::endl;
    }
}

void Disk::read(std::string fileName) {
    if (currentDirectory->children.find(fileName) != currentDirectory->children.end() &&
        !currentDirectory->children[fileName]->isDirectory) {
        std::cout << currentDirectory->children[fileName]->content << std::endl;
    } else {
        std::cout << "File not found" << std::endl;
    }
}

void Disk::write(std::string fileName, std::string content) {
    if (currentDirectory->children.find(fileName) != currentDirectory->children.end() &&
        !currentDirectory->children[fileName]->isDirectory) {
        currentDirectory->children[fileName]->content = content;
        currentDirectory->children[fileName]->size = content.size();
        currentDirectory->children[fileName]->dataStartBlock = reinterpret_cast<size_t>(&currentDirectory->children[fileName]->content);
    } else {
        std::cout << "File not found" << std::endl;
    }
}

void Disk::exit() {
    std::cout << "Exiting file system..." << std::endl;
}
