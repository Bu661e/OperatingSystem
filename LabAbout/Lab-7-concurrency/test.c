#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 6			
typedef int buffer_item;		
buffer_item buffer[BUFFER_SIZE];


HANDLE empty;    // 表示缓冲区空槽的信号量
HANDLE full;     // 表示缓冲区满槽的信号量
HANDLE mutex;    // 互斥信号量，保护对缓冲区的访问


int insert_position = 0;
int remove_position = 0;

void put (buffer_item value)
{
	buffer[insert_position] = value;
	insert_position = (insert_position + 1) % BUFFER_SIZE;

	printf("producer(%lu) puts %d\n", GetCurrentThreadId(), value);
}

void get ()
{
	int tmp = buffer[remove_position];
	remove_position = (remove_position + 1) % BUFFER_SIZE;

	printf("consumer(%lu) gets %d\n", GetCurrentThreadId(), tmp);
	return tmp;
}

DWORD WINAPI producer(LPVOID param) {
	buffer_item item;
	srand(GetCurrentThreadId());
	while (TRUE) {
		Sleep(rand() % 1000); // 模拟生产时间
		
		item = rand(); // 生成一个随机项
		

		WaitForSingleObject(empty, INFINITE); // 等待缓冲区空槽
		WaitForSingleObject(mutex, INFINITE); // 进入临界区

//		// 生产数据
		put(item);
		
		ReleaseSemaphore(mutex, 1, NULL); // 离开临界区
		ReleaseSemaphore(full, 1, NULL);  // 增加缓冲区满槽

	}
	return 0;
}

DWORD WINAPI consumer(LPVOID param) {
	
	while (TRUE) {
		Sleep(rand() % 1000); // 模拟消费时间
		
		WaitForSingleObject(full, INFINITE); // 等待缓冲区满槽
		WaitForSingleObject(mutex, INFINITE); // 进入临界区

		// 消费数据
		get();
		
		ReleaseSemaphore(mutex, 1, NULL); // 离开临界区
		ReleaseSemaphore(empty, 1, NULL); // 增加缓冲区空槽
	}
	return 0;
}

int main(int argc, char *argv[]) {
	
	int sleep_time = 10;
	
	// 创建信号量
	empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
	full = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
	mutex = CreateSemaphore(NULL, 1, 1, NULL);
	
	
	// argv[0] 是程序名称，所以我们从 argv[1] 开始读取用户传入的参数
	int arg1 = atoi(argv[1]); // 将字符串转换为整数
	int arg2 = atoi(argv[2]); // 同上
	
//	// 创建生产者线程

	while (arg1--)
	{
		CreateThread(NULL, 0, producer, NULL, 0, NULL);
	}
	
	// 创建消费者线程

	while (arg2--)
	{
		CreateThread(NULL, 0, consumer, NULL, 0, NULL);
	}
	


	// 主线程休眠一段时间
	Sleep(sleep_time * 1000);
	
	// 清理资源
	CloseHandle(empty);
	CloseHandle(full);
	CloseHandle(mutex);
	
	return 0;
}

