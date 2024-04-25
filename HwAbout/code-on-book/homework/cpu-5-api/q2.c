#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define FILE_PATH "./shared_file.txt"

int main() {

    int fd = open("./q2.txt", O_WRONLY | O_CREAT | O_APPEND);

    if (fd == -1) {
        printf("open failed");
        exit(1);
    }

    int rc = fork();
    if (rc < 0) {
        perror("fork");
        exit(0);
    } else if (rc == 0) {  // 子进程
        char str1[]  = "1234567890";

        write(fd, str1, sizeof(str1));
    } else {  // 父进程
        char str2[]  = "abcdefghigklmnopqrstuvwxyz";

        write(fd, str2, sizeof(str2));

    }

    close(fd);  
    return 0;
}