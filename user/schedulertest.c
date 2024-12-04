#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define NFORK 10
#define IO 5

int main()
{
  int n, pid;
  int wtime, rtime;
  int twtime = 0, trtime = 0;
  for (n = 0; n < NFORK; n++)
  {
    pid = fork();
    if (pid < 0)
      break;
    if (pid == 0)
    {
      // settickets(1); // Set tickets for this process
      if (n < IO)
      {
        sleep(200); // IO bound processes
      }
      else
      {
        // settickets(10);
        for (volatile int i = 0; i < 1000000000; i++)
        {
        } // CPU bound process
      }
      exit(0);
    }
  }
  for (; n > 0; n--)
  {
    if (waitx(0, &wtime, &rtime) >= 0)
    {
      trtime += rtime;
      twtime += wtime;
      // printf("Process %d: rtime %d, wtime %d\n", n, rtime, wtime);
    }
  }
  printf("Average rtime %d,  wtime %d\n", trtime / NFORK, twtime / NFORK);
  exit(0);
}

// // FOR LBS


// // #include "kernel/types.h"
// // #include "kernel/stat.h"
// // #include "user/user.h"
// // #include "kernel/fcntl.h"

// // #define NFORK 10
// // #define IO 5

// // // Use a file for synchronization
// // #define SYNC_FILE "sync_file"

// // void print_sync(const char *s) {
// //   int fd = open(SYNC_FILE, O_CREATE | O_RDWR);
// //   if (fd < 0) {
// //     exit(1);
// //   }
  
// //   // Acquire lock
// //   while(link(SYNC_FILE, "lock") < 0);
  
// //   // Write to stdout
// //   write(1, s, strlen(s));
  
// //   // Release lock
// //   unlink("lock");
// //   close(fd);
// // }

// // void printint_sync(int x) {
// //   char buf[16];
// //   int i = 0;
// //   do {
// //     buf[i++] = x % 10 + '0';
// //     x /= 10;
// //   } while (x > 0);
  
// //   char output[32];
// //   int j = 0;
// //   for (int k = i - 1; k >= 0; k--) {
// //     output[j++] = buf[k];
// //   }
// //   output[j] = '\0';
  
// //   print_sync(output);
// // }

// // int main()
// // {
// //   int n, pid;
// //   int wtime, rtime;
// //   int twtime = 0, trtime = 0;
  
// //   // Create sync file
// //   close(open(SYNC_FILE, O_CREATE));
  
// //   for (n = 0; n < NFORK; n++)
// //   {
// //     pid = fork();
// //     if (pid < 0)
// //       break;
// //     if (pid == 0)
// //     {
// //       int tickets = n + 1;
// //       settickets(tickets); // Set tickets for this process
      
// //       if (n < IO)
// //       {
// //         print_sync("IO bound process ");
// //         printint_sync(n);
// //         print_sync(" created with 1 ticket, then set to ");
// //         printint_sync(tickets);
// //         print_sync(" tickets\n");
// //         sleep(200); // IO bound processes
// //       }
// //       else
// //       {
// //         print_sync("CPU bound process ");
// //         printint_sync(n);
// //         print_sync(" created with 1 ticket, then set to ");
// //         printint_sync(tickets);
// //         print_sync(" tickets\n");
// //         for (volatile int i = 0; i < 1000000000; i++)
// //         {
// //         } // CPU bound process
// //       }
// //       print_sync("Process ");
// //       printint_sync(n);
// //       print_sync(" finished\n");
// //       exit(0);
// //     }
// //     else
// //     {
// //       print_sync("Parent created child ");
// //       printint_sync(n);
// //       print_sync(" with PID ");
// //       printint_sync(pid);
// //       print_sync("\n");
// //     }
// //   }
  
// //   for (; n > 0; n--)
// //   {
// //     if (waitx(0, &wtime, &rtime) >= 0)
// //     {
// //       print_sync("Child ");
// //       printint_sync(n-1);
// //       print_sync(": rtime ");
// //       printint_sync(rtime);
// //       print_sync(", wtime ");
// //       printint_sync(wtime);
// //       print_sync("\n");
// //       trtime += rtime;
// //       twtime += wtime;
// //     }
// //   }
  
// //   print_sync("Average rtime ");
// //   printint_sync(trtime / NFORK);
// //   print_sync(",  wtime ");
// //   printint_sync(twtime / NFORK);
// //   print_sync("\n");
  
// //   // Clean up
// //   unlink(SYNC_FILE);
// //   exit(0);
// // }


// // #include "kernel/types.h"
// // #include "kernel/stat.h"
// // #include "user/user.h"
// // #include "kernel/fcntl.h"

// // #define NFORK 10
// // #define IO 5

// // void spin()
// // {
// //   for(int i = 0; i < 100000000; i++)
// //   {
// //     asm("nop");
// //   }
// // }

// // int main()
// // {
// //   int n, pid;
// //   int wtime, rtime;
// //   int twtime = 0, trtime = 0;
// //   int start_time, end_time;

// //   for (n = 0; n < NFORK; n++)
// //   {
// //     pid = fork();
// //     if (pid < 0)
// //       break;
// //     if (pid == 0)
// //     {
// //       start_time = uptime();
// //       if (n < IO)
// //       {
// //         // IO bound processes
// //         for(int i = 0; i < 100; i++)
// //         {
// //           sleep(1);
// //           if(i % 10 == 0)
// //           {
// //             printf("IO Process %d: PID=%d, Time=%d\n", n, getpid(), uptime() - start_time);
// //           }
// //         }
// //       }
// //       else
// //       {
// //         // CPU bound processes
// //         for(int i = 0; i < 100; i++)
// //         {
// //           spin();
// //           if(i % 10 == 0)
// //           {
// //             printf("CPU Process %d: PID=%d, Time=%d\n", n, getpid(), uptime() - start_time);
// //           }
// //         }
// //       }
// //       end_time = uptime();
// //       printf("Process %d finished. Total time: %d\n", n, end_time - start_time);
// //       exit(0);
// //     }
// //     else
// //     {
// //       printf("Parent created child %d with PID %d\n", n, pid);
// //     }
// //   }

// //   for (; n > 0; n--)
// //   {
// //     if (waitx(0, &wtime, &rtime) >= 0)
// //     {
// //       printf("Child %d: rtime %d, wtime %d\n", n-1, rtime, wtime);
// //       trtime += rtime;
// //       twtime += wtime;
// //     }
// //   }
// //   printf("Average rtime %d, wtime %d\n", trtime / NFORK, twtime / NFORK);
// //   exit(0);
// // }

// #include "kernel/param.h"
// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "kernel/riscv.h"
// #include "user/user.h"
// #pragma GCC push_options
// #pragma GCC optimize ("O0") // Causing wierd errors of moving things here and there

// void usless_work() {
//     for (int i = 0; i < 1000 * 900000; i++) {
//         asm volatile("nop"); // avoid compiler optimizing away loop
//     }
// }

// void test0(){
//     settickets(600);// So that parent will get the higher priority and the forks can run at once
//     printf("TEST 0\n"); // To check the randomness
//     int prog1_tickets = 20;
//     int prog2_tickets = 40;
//     int prog3_tickets = 2;
//     int prog4_tickets = 80;
//     printf("Child 1 has %d tickets.\nChild 2 has %d tickets\nChild 3 has %d tickets\nChild 4 has %d tickets\n",
//            prog1_tickets, prog2_tickets, prog3_tickets, prog4_tickets);

//     if (fork() == 0) {
//         printf("Child 1 started\n");
//         sleep(1);
//         settickets(prog1_tickets);
//         usless_work();
//         printf("Child 1 exited\n");
//         exit(0);

//     }
//     if (fork() == 0) {
//         printf("Child 2 started\n");
//         sleep(1);
//         settickets(prog2_tickets);
//         usless_work();
//         printf("Child 2 exited\n");
//         exit(0);
//     }
//     if (fork() == 0) {
//         printf("Child 3 started\n");
//         sleep(1);
//         settickets(prog3_tickets);
//         usless_work();
//         printf("Child 3 exited\n");
//         exit(0);
//     }
//     if (fork() == 0) {
//         printf("Child 4 started\n");
//         sleep(1);
//         settickets(prog4_tickets);
//         usless_work();
//         printf("Child 4 exited\n");
//         exit(0);
//     }
//     wait(0);
//     wait(0);
//     wait(0);
//     wait(0);
//     printf("The correct order should be ideally 4,2,1 and then 3.\n");

// }

// void test1(){
//     printf("TEST1\n"); // To check the FCFS part of the implementation
//     int tickets = 30; // To check for this finish times
//     settickets(30); // So that now, the parent will always get the main priority to set up its children
//     sleep(1); // So that this will have a different ctime than others. Ctime is not entirely very accurate

//     printf("Child 1 started\n");
//     if (fork() == 0) {
//         settickets(tickets);
//         usless_work();
//         printf("Child 1 ended\n");
//         exit(0);
//     }
//     printf("Child 2 started\n");
//     if (fork() == 0) {
//         settickets(tickets);
//         usless_work();
//         printf("Child 2 ended\n");
//         exit(0);
//     }
//     printf("Child 3 started\n");
//     if (fork() == 0) {
//         settickets(tickets);
//         usless_work();
//         printf("Child 3 ended\n");
//         exit(0);
//     }
//     wait(0);
//     wait(0);
//     wait(0);
//     printf("The order should be 1,2 and then 3 since all tickets have same value\n");
// }
// int main() {
//     test0();
//     test1();
//     printf("Finished all tests\n");

//     return 0;
// }