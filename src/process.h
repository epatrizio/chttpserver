#ifndef PROCESS_H
#define PROCESS_H

// Full-duplex pipe
typedef struct {
	int parent[2];  // 0:read - 1:write
	int child[2];
} FD_pipe;

void wait_pid(pid_t);

#endif