// File.h
#ifndef FILE_H
#define FILE_H

#include <string>
#include <map>

class File {
public:
    std::string name; // 文件或目录的名称
    bool isDirectory; // 文件类型：true 表示目录，false 表示文件
    size_t size; // 文件大小（字节数），对于目录可以表示包含的文件和子目录数量
    std::string content; // 文件内容，仅适用于文件
    std::map<std::string, File*> children; // 子文件和子目录，仅适用于目录
    void* FCB; // 文件控制块指针，指向当前File对象
    size_t dataStartBlock; // 数据起始块，在内存文件系统中可以是文件内容字符串的起始地址

    File(std::string name, bool isDirectory);
    ~File();
};

#endif // FILE_H
