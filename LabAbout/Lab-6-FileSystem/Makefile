# Makefile

# 编译器设置
CXX = g++
CXXFLAGS = -Wall -g

# 目标文件
TARGET = my_shell

# 源文件
SRCS = my_shell.cpp Disk.cpp File.cpp

# 生成的对象文件
OBJS = $(SRCS:.cpp=.o)

# 规则：生成目标文件
all: $(TARGET)

# 规则：生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# 规则：生成 my_shell.o
my_shell.o: my_shell.cpp Disk.h File.h
	$(CXX) $(CXXFLAGS) -c my_shell.cpp

# 规则：生成 Disk.o
Disk.o: Disk.cpp Disk.h File.h
	$(CXX) $(CXXFLAGS) -c Disk.cpp

# 规则：生成 File.o
File.o: File.cpp File.h
	$(CXX) $(CXXFLAGS) -c File.cpp

# 规则：清理生成的文件
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
