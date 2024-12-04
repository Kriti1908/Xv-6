#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/syscall.h"
#include "user/syscount.h"

int getsyscount(int pid, int mask); // Declaration for getsyscount

// Map syscall mask to system call names
char *get_syscall_name_from_mask(int mask)
{
    switch (mask)
    {
    case (1 << SYS_fork):
        return "fork";
    case (1 << SYS_exit):
        return "exit";
    case (1 << SYS_wait):
        return "wait";
    case (1 << SYS_pipe):
        return "pipe";
    case (1 << SYS_read):
        return "read";
    case (1 << SYS_kill):
        return "kill";
    case (1 << SYS_exec):
        return "exec";
    case (1 << SYS_fstat):
        return "fstat";
    case (1 << SYS_chdir):
        return "chdir";
    case (1 << SYS_dup):
        return "dup";
    case (1 << SYS_getpid):
        return "getpid";
    case (1 << SYS_sbrk):
        return "sbrk";
    case (1 << SYS_sleep):
        return "sleep";
    case (1 << SYS_uptime):
        return "uptime";
    case (1 << SYS_open):
        return "open";
    case (1 << SYS_write):
        return "write";
    case (1 << SYS_mknod):
        return "mknod";
    case (1 << SYS_unlink):
        return "unlink";
    case (1 << SYS_link):
        return "link";
    case (1 << SYS_mkdir):
        return "mkdir";
    case (1 << SYS_close):
        return "close";
    case (1 << SYS_waitx):
        return "waitx";
    case (1 << SYS_getsyscount):
        return "getsyscount";
    default:
        return "unknown";
    }
}

int get_syscall_num_from_mask(int mask) 
{
 switch (mask)
    {
    case (1 << SYS_fork):
        return SYS_fork;
    case (1 << SYS_exit):
        return SYS_exit;
    case (1 << SYS_wait):
        return SYS_wait;
    case (1 << SYS_pipe):
        return SYS_pipe;
    case (1 << SYS_read):
        return SYS_read;
    case (1 << SYS_kill):
        return SYS_kill;
    case (1 << SYS_exec):
        return SYS_exec;
    case (1 << SYS_fstat):
        return SYS_fstat;
    case (1 << SYS_chdir):
        return SYS_chdir;
    case (1 << SYS_dup):
        return SYS_dup;
    case (1 << SYS_getpid):
        return SYS_getpid;
    case (1 << SYS_sbrk):
        return SYS_sbrk;
    case (1 << SYS_sleep):
        return SYS_sleep;
    case (1 << SYS_uptime):
        return SYS_uptime;
    case (1 << SYS_open):
        return SYS_open;
    case (1 << SYS_write):
        return SYS_write;
    case (1 << SYS_mknod):
        return SYS_mknod;
    case (1 << SYS_unlink):
        return SYS_unlink;
    case (1 << SYS_link):
        return SYS_link;
    case (1 << SYS_mkdir):
        return SYS_mkdir;
    case (1 << SYS_close):
        return SYS_close;
    case (1 << SYS_waitx):
        return SYS_waitx;
    case (1 << SYS_getsyscount):
        return SYS_getsyscount;
    default:
        return -1;
    }   
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: syscount <mask> command [args]\n");
        exit(0);
    }

    int mask = atoi(argv[1]);
    // int pipe_fds[2]; // Pipe for synchronization
    // pipe(pipe_fds);   // Create the pipe

    int pid = fork();
    if (pid < 0)
    {
        printf("fork failed\n");
        exit(0);
    }

    if (pid == 0)
    {
        // In the child process: execute the command
        // close(pipe_fds[0]); // Close the read end of the pipe

        // Notify parent before exec
        // write(pipe_fds[1], "ready", 5);  // Signal parent that exec is about to run

        exec(argv[2], &argv[2]);
        printf("exec failed\n "); // Only executed if exec fails
        exit(0);
    }
    else
    {
        // In the parent process
        // close(pipe_fds[1]); // Close the write end of the pipe

        // Wait for the child to signal that it's about to run exec
        // char buf[5];
        // read(pipe_fds[0], buf, 5);  // Read the signal from the child
        // close(pipe_fds[0]);         // Close the read end of the pipe after receiving the signal

        int status;
        wait(&status);
        // Get the syscall count
        int count = getsyscount(pid, mask);
        if (count < 0)
        {
            printf("getsyscount failed\n");
            exit(0);
        }

        
        // Get the syscall name from the mask
        char *syscall_name = get_syscall_name_from_mask(mask);
        printf("PID %d called %s %d times.\n", pid, syscall_name, count);
        // int syscall_pid = get_syscall_num_from_mask(mask);
        // global_syscall_count[pid][syscall_pid] = 0;

        // Wait for the child to finish execution
    }

    exit(0);
}