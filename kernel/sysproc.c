#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "user/syscount.h"

extern int syscall_count[1000000][100000];

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_waitx(void)
{
  uint64 addr, addr1, addr2;
  uint wtime, rtime;
  argaddr(0, &addr);
  argaddr(1, &addr1); // user virtual memory
  argaddr(2, &addr2);
  int ret = waitx(addr, &wtime, &rtime);
  struct proc *p = myproc();
  if (copyout(p->pagetable, addr1, (char *)&wtime, sizeof(int)) < 0)
    return -1;
  if (copyout(p->pagetable, addr2, (char *)&rtime, sizeof(int)) < 0)
    return -1;
  return ret;
}

// Try fetching arguments directly with argint
int sys_getsyscount(void)
{
    int pid, mask;
    // printf("Entering sys_getsyscount...\n");
    // Fetch arguments using argint
    argint(0, &pid);
    argint(1, &mask);
    // printf("sys_getsyscount: After argint, pid = %d, mask = %d\n", pid, mask);

    if (pid < 0 || pid >= MAX_PID) {
        printf("Invalid PID %d\n", pid);
        return -1;
    }

    // struct proc *p = 0;
    // // Find the process with the given pid
    // for(p = proc; p < &proc[NPROC]; p++) {
    //     if(p->pid == pid) {
    //         break;
    //     }
    // }
    // if (p == 0 || p->pid != pid) {
    //     printf("Process with pid %d not found\n", pid);
    //     return -1;
    // }

    int syscall_index = -1;
    // Determine which system call to track based on the mask
    for (int i = 0; i < 32; i++) {
        if ((mask >> i) & 1) {
            syscall_index = i;
            break;
        }
    }

    if (syscall_index == -1) {
        printf("Invalid mask, exiting sys_getsyscount\n");
        return -1; // Invalid mask
    }

    // Retrieve the syscall count for the specified process
    // int total_count = p->syscall_count[syscall_index];
    // printf("Total count for syscall index %d: %d\n", syscall_index, total_count);
    // return total_count;
    int total_count = global_syscall_count[pid][syscall_index];
    global_syscall_count[pid][syscall_index] = 0;
    // printf("Total count for syscall index %d: %d\n", syscall_index, total_count);
    return total_count;
}

uint64
sys_sigalarm(void)
{
  int ticks;
  uint64 handler;

  argint(0, &ticks);
  argaddr(1, &handler);

  struct proc *p = myproc();
  p->alarmticks = ticks;
  p->ticksleft = ticks;
  p->alarmhandler = (void(*)())handler;
  p->is_alarm_on = (ticks > 0);

  return 0;
}

uint64
sys_sigreturn(void)
{
    struct proc *p = myproc();
    if (p->backup_trapframe) {
        memmove(p->trapframe, p->backup_trapframe, sizeof(struct trapframe));
        kfree(p->backup_trapframe);
        p->backup_trapframe = 0;
        p->alarm_in_progress = 0;  // Reset the flag
    }
    usertrapret();
    return 0;
}

uint64
sys_settickets(void)
{
  int n;
  argint(0, &n);
  if(n < 1)
    return -1;
  
  struct proc *p = myproc();
  acquire(&p->lock);
  p->tickets = n;
  release(&p->lock);
  return 0;
}