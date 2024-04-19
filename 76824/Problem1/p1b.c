#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t childPid;

    childPid = fork();

    if (childPid > 0) {
        printf("Parent process (zombie) created child with PID: %d\n", childPid);
        sleep(5);
    }
    else if (childPid == 0) {
        printf("Child process with PID: %d\n", getpid());
        exit(0);
    }

    return 0;
}

