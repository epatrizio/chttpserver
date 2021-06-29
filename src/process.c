#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "process.h"

void wait_pid(pid_t pid)
{
	int status;

    if (waitpid(pid, &status, 0) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status))
            printf("Process %u execution successful!\n", pid);
        else if (WIFEXITED(status) && WEXITSTATUS(status)) {
            if (WEXITSTATUS(status) == 127)
                perror("Process execution failed!\n");
            else
                printf("Process %u terminated normally, but returned a non-zero status\n", pid);
        }
        else
            perror("Process didn't terminate normally\n"); 
    }
    else
        perror("waitpid() failed!");
}
