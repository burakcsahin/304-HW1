#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <float.h>

void ExecuteChildCommand(char* command, int pipefd[2], int child_id) {
    // /dev/null
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(pipefd[0], STDOUT_FILENO);
        execlp(command, "ls", "-la", NULL);
        exit(1);
    } 
    else if (pid > 0) {
        wait(NULL);
    } 

    gettimeofday(&end, NULL);
    double execution_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000.0;
    write(pipefd[1], &execution_time, sizeof(execution_time));
}

int main(int argc, char* argv[]) {

    int n = atoi(argv[1]);
    pid_t children[n];
    int pipefd[n][2];

    // Create pipe
    for (int i = 0; i < n; i++) {
        if (pipe(pipefd[i]) == -1) {
            return 1;
        }
    }

    // Fork processes
    for (int i = 0; i < n; i++) {
        children[i] = fork();
        
	if (children[i] == 0) {
           
            ExecuteChildCommand(argv[2], pipefd[i], i + 1);
            return 0;
        }
       	else if(children[i] >0) {
          
            close(pipefd[i][1]);
        }
    }

    double execTimes[n];
    int childStatus;
    int i = 0;
    while (i < n) {
        waitpid(children[i], &childStatus, 0);
        read(pipefd[i][0], &execTimes[i], sizeof(execTimes[i]));
        close(pipefd[i][0]);
        printf("Child %d Executed in %.2f millis\n", i + 1, execTimes[i]);
	i += 1;
    }

    
    double minTime = 99999999999;
    double maxTime = 0;
    double sumTime = 0;
    int count = 0;

    while (count < n) {
        sumTime += execTimes[count];
        if (execTimes[count] < minTime) {
            minTime = execTimes[count];
        }
	else if (execTimes[count] > maxTime) {
            maxTime = execTimes[count];
        }
        count++;
    }

    printf("Max: %.2f millis\n", maxTime);
    printf("Min: %.2f millis\n", minTime);
    printf("Average: %.2f millis\n", sumTime / n);

    return 0;
}
