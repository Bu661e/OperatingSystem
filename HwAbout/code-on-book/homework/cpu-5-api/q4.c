#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	char * s = "/bin/ls";
	char * s1 = "ls";
	char * s2 = "/";
	char * str_vec[] = { s1, s2, NULL };
	for(int i = 0;i < 6; ++i) {
		int rc = fork();
		if (rc < 0) {
			printf("fork failed");
			exit(1);
		} else if (rc == 0) {
			switch(i) {
			case 0:
				execl(s, s1, s2, NULL);
				break;
			case 1:
				execle(s, s1, s2, NULL);
				break;
			case 2:
				execlp(s, s, s2, NULL);
				break;
			case 3:
				execv(s, str_vec);
				break;
			case 4:
				execvp(s1, str_vec);
				break;
			case 5:
				execvpe(s1, str_vec);
				break;
			default: break;
			}
		} else {
			wait(NULL);
		}
	}
	return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>

// int main(int argc, char *argv[]) {
//     printf("hello world (pid:%d)\n", (int) getpid());
//     int rc = fork();

//     if (rc < 0) {
//         fprintf(stderr, "fork failed\n");
//         exit(1);
//     } else if (rc == 0) {

//         printf("hello, I am child (pid:%d)\n", (int) getpid());

//         char *args[] = { "ls", NULL };
//         char *envp[] = { NULL };


//         // printf("execl\n");
//         // execl("/bin/ls", "ls", NULL);

//         printf("execle\n");
//         execle("/bin/ls", "ls", NULL, envp);

//         printf("execlp\n");
//         execlp("ls", "ls", NULL);

//         printf("execv\n");
//         execv("/bin/ls", args);

//         printf("execvp\n");
//         execvp("ls", args);

//     } else {
//         int wc = wait(NULL);
//         printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
// 	       rc, wc, (int) getpid());
//     }
//     return 0;
// }
