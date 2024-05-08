#include <stdio.h>
#include <sys/mman.h>
#include <stdbool.h>

#define BUF_4KB 4 * 1024
#define BUF_1MB 1 * 1024 * 1024
#define BUF_4MB 4 * 1024 * 1024
#define BUF_256MB 256 * 1024 * 1024

#define HEAP_SUCCESS 1
#define HEAP_FAILURE -1

//1个内存块block由1个头块chunk和被分配的空闲内存组成

//头块 在64位机器中大小为4字节
struct chunk_t
{
	size_t size;			//该chunk对应的block中空闲内存的大小
	
	unsigned isFree;		// isFree = 1表示该内存块空闲
							// 这里用unsigned是为了保持chunk_t保持4字节对齐

	struct chunk_t* next;	//指向下一个chunk，也就是
	
	struct chunk_t* pre;	//这里头块之间用双向链表连接，方便后续合并
};

struct heap_t
{
	struct chunk_t* start;
	struct chunk_t* end;
	size_t avail;
	size_t total;
};

struct heap_t newHeap = {0};

bool isValid(size_t size)
{
	if(size == 4 * 1024) return true;
	if(size == 1 * 1024 * 1024) return true;
	if(size == 4 * 1024 * 1024) return true;
	return false;
}

void* getTar(size_t size)
{
	struct chunk_t* tar = newHeap.start;
	while (tar)
	{
		
		if(tar->isFree && tar->size >= size + sizeof(struct chunk_t))
		{
			return tar;
		}
		tar = tar->next;
	}	
	return NULL;
}

int initHeap()
{
	
	// 使用 mmap 分配内存
	void* addr = mmap(NULL, BUF_256MB, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) {
		perror("mmap申请内存失败: ");
		return HEAP_FAILURE;
	}
	
	
	struct chunk_t* chunk = addr;
	chunk->isFree = 1;
	chunk->size = BUF_256MB - sizeof(struct chunk_t);
	chunk->next = NULL;
	chunk->pre = NULL;
	
	
	newHeap.start = chunk;
	newHeap.end = chunk;
	newHeap.avail = chunk->size;
	
	return HEAP_SUCCESS;
}

void* emalloc(size_t size)
{
	if (!isValid(size))
	{
		printf("对不起，您申请的空间不满足要求，仅支持4KB、1MB、4MB大小的内存申请\n");
		return NULL;		
	}
	if (size > newHeap.avail)
	{
		printf("对不起，您申请的空间已经超过已有空闲空间\n");
		return NULL;
	}
	
	struct chunk_t* tar = getTar(size);
	if(tar == NULL)
	{
		printf("对不起，已有空间不足\n");
		return NULL;
	}
	
	struct chunk_t* newChunk = (void*)(tar + 1 + size/sizeof(struct chunk_t) );
	
	
	newChunk->size = tar->size - size - sizeof(struct chunk_t);
	newChunk->isFree = 1;
	tar->size = size;
	tar->isFree = 0;
	
	newChunk->next = tar->next;
	if(tar != newHeap.end)
	{
		tar->next->pre = newChunk;	
	}
	else
	{
		newHeap.end = newChunk;
	}

	tar->next = newChunk;
	newChunk->pre = tar;
	
	newHeap.avail -= sizeof(struct chunk_t) + size;
	
	return (void*)(tar + 1);
	
	
}

void efree(void* ptr)
{
	//这里由于ptr是void*，做地址乘法时，基数是1
	struct chunk_t* tar = (struct chunk_t*)(ptr - sizeof(struct chunk_t));
	
	newHeap.avail += tar->size + sizeof(struct chunk_t);
	
	struct chunk_t* n = tar->next;
	struct chunk_t* p = tar->pre;
	tar->isFree = 1;
	
	if(n->isFree)
	{
		tar->size += n->size + sizeof(struct chunk_t);
		n->next->pre = tar;
		tar->next = n->next;
	}
	
	if(p->isFree)
	{
		p->size += tar->size + sizeof(struct chunk_t);
		tar->next->pre = p;
		p->next = tar->next;
	}
	
}

void eprint()
{
	struct chunk_t* tar = newHeap.start;
	int idx = 1;
	while (tar)
	{
		printf("idx = %d \tsize = %#lx   \tisFree = %u\tpre = %p     \t now = %p   \t next = %p\n", idx++, tar->size, tar->isFree, tar->pre, tar, tar->next);
		tar = tar->next;
	}
		printf("\n");

}

void test()	// 测试emalloc
{
	printf("sizeof(chunk_t) = %#x\n", (int)sizeof(struct chunk_t));
	printf("newHeap.start  = %p\n\n", newHeap.start);

	printf("--1. emalloc前--\n");
	eprint();

	printf("--2. emalloc后--\n");

	void* ptr1 = emalloc(BUF_1MB);
	void* ptr2 = emalloc(BUF_4MB);
	void* ptr3 = emalloc(BUF_4KB);
	void* ptr4 = emalloc(BUF_1MB);
	void* ptr5 = emalloc(BUF_4MB);
	void* ptr6 = emalloc(BUF_4KB);
	void* ptr7 = emalloc(BUF_4KB);
	void* ptr8 = emalloc(BUF_4KB);
	void* ptr9 = emalloc(BUF_4KB);
	eprint();

	printf("ptr1(%#x) = %p\n", BUF_1MB, ptr1);
	printf("ptr2(%#x) = %p\n", BUF_4MB, ptr2);
	printf("ptr3(%#x)   = %p\n\n", BUF_4KB, ptr3);

	printf("--3. efree ptr4 & ptr6后--\n");
	efree(ptr4);
	efree(ptr6);

	eprint();

	printf("--4. efree ptr5后--\n");
	efree(ptr5);
	eprint();




	

}

int main()
{
	if(initHeap() == HEAP_FAILURE)
	{
		printf("堆申请失败\n");
		return -1;
	}

	test();
	munmap(newHeap.start, newHeap.total);
	
	return 0;
}

