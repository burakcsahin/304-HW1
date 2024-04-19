#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void processCreator(int n) {
        pid_t pid;
        pid_t parent;
        int level = 0;
        parent = getppid();
        int i;

        printf("Main Process ID: %d, level: %d\n", getpid(), level++);

        for(i = 0; i < n; i++) {
                pid = fork();

                if (pid == 0) { /* child process */
                        printf("Main Process ID: %d, Parent ID: %d, level: %d \n", getpid(), parent, level++);
                        parent = getpid();
                }

        }
        for(int i=0;i<n;i++) {
                wait(NULL);
        }
}

int main(int argc, char *argv[])
{
        if (argc != 2) {
                fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
                exit(1);
        }

        int n = atoi(argv[1]);
        processCreator(n);
        return 0;
}
