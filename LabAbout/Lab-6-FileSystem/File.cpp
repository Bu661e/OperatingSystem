// File.cpp
#include "File.h"

File::File(std::string name, bool isDirectory) : name(name), isDirectory(isDirectory), size(0), FCB(this), dataStartBlock(0) {
    if (!isDirectory) {
        // 假设每个文件的数据起始块为其内容的地址
        dataStartBlock = reinterpret_cast<size_t>(&content);
    }
}

File::~File() {
    for (auto& child : children) {
        delete child.second;
    }
}
