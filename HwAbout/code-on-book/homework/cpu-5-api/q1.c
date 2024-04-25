#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    int demo = 0;
    demo = 1;
    int rc = fork();
    if (rc < 0) { //fork 失败
        printf("fork failed");
        exit(1);
    } else if (rc == 0) { //子进程
        printf("这里是子进程， 修改前demo = %d\n", demo);
        demo = getpid();
        printf("这里是子进程， 修改后demo = %d\n", demo);

    } else { //父进程
        printf("这里是父进程， 修改前demo = %d\n", demo);
        demo = getpid();
        printf("这里是父进程， 修改后demo = %d\n", demo);
    }
    
    return 0;
}
