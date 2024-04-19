#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to search for a number in a portion of the array
void search_number(int *arr, int start, int end, int x) {
    for (int i = start; i <= end; i++) {
        if (arr[i] == x) {
            printf("Found at index %d\n", i);
            exit(0);
        }
    }
    exit(1);
}

int main(int argc, char *argv[]) {
   
    int x = atoi(argv[1]);
    int n = atoi(argv[2]);

  
    int *numbers = malloc(sizeof(int) * 1000);
    int numsRead = 0;

    int num;
    while (scanf("%d", &num) == 1) {
        numbers[numsRead++] = num;
    }

    
    int portionSize = numsRead / n;
    int remainder = numsRead % n; 

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == 0) { 
            int start = i * portionSize;
	    int end;
	    if (i == n - 1) {
    	       end = start + portionSize + remainder - 1;
	    }
	    else {
    	       end = start + portionSize - 1;
	    }

            search_number(numbers, start, end, x);
        } 
	else if (pid < 0) { 
            free(numbers); 
            return 1;
        }
    }

    
    int status;
    pid_t child_pid = wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        //kill children
        for (int i = 0; i < n; i++) {
            if (getpid() != child_pid) {
                kill(getpid(), SIGKILL);
            }
        }
    }

    free(numbers); // Free allocated memory
    return 0;
}

