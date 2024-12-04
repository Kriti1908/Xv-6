
# Enhanced xv6 Kernel Features and Schedulers

Welcome to the enhanced xv6 kernel! This project extends the traditional xv6 operating system with additional system calls and advanced scheduling policies. Below is a comprehensive guide to the features and how to use them.

---

## Features Overview

### 1. **`syscount` - Tracking System Call Usage**
   - **Description**: Monitors how many times specific system calls are invoked by a process and its children during execution.
   - **Key Features**:
     - Supports tracking system calls via a bitmask.
     - Provides syscall usage counts, enabling performance analysis and debugging.
     - Displays detailed outputs including the process ID, syscall name, and the count of invocations.
   - **Use Case**: Developers can analyze system call behaviors in their programs, especially useful for performance profiling.

### 2. **`sigalarm` and `sigreturn` - Timed Function Execution**
   - **`sigalarm`**:
     - Sets a periodic alarm that triggers a user-defined function after every `n` CPU ticks.
   - **`sigreturn`**:
     - Restores the process state to resume normal execution after the alarm handler finishes.
   - **Key Features**:
     - Allows pre-emptive user-defined function execution.
     - Ensures consistent and error-free state restoration after signal handling.
   - **Use Case**: Enables timed operations and periodic task execution within user programs.

### 3. **Advanced Scheduling Policies**
   - The kernel supports three schedulers, each catering to specific use cases:
     1. **Round-Robin Scheduler (RR)**:
        - Default scheduling policy.
        - Simple time-sharing mechanism, ensuring equal CPU time slices for all processes.
     2. **Lottery-Based Scheduler (LBS)**:
        - Assigns CPU time proportional to the number of tickets each process owns.
        - **Key Features**:
          - Fair random process selection based on tickets.
          - Ensures tie-breaking using process arrival time.
          - Processes inherit parent tickets during creation.
        - **Use Case**: Suitable for scenarios requiring probabilistic fairness.
     3. **Multilevel Feedback Queue (MLFQ) Scheduler**:
        - Dynamically adjusts process priority based on behavior.
        - **Key Features**:
          - Boosts priority periodically to prevent starvation.
          - Aging mechanism ensures fairness for long-waiting processes.
          - Differentiates between CPU-bound and I/O-bound processes for better throughput.
        - **Use Case**: Optimized for workloads with a mix of CPU and I/O-bound tasks.

---

## Installation and Setup

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/<your-repo>/enhanced-xv6.git
   ```
2. **Navigate to the Project Directory**:
   ```bash
   cd enhanced-xv6
   ```
3. **Build the Kernel**:
   - Use the `make` command to compile the kernel and create the xv6 image:
     ```bash
     make clean && make
     ```
4. **Run xv6**:
   - Launch the xv6 kernel using QEMU:
     ```bash
     make qemu
     ```
5. **Test New Features**:
   - Run specific user programs in the `user` directory to test features like `syscount` and `sigalarm`.

---

## Detailed Feature Explanation

### **Schedulers in Depth**
- **Round-Robin (RR)**:
  - Ensures a simple, fair division of CPU time.
  - Best suited for low-overhead, evenly distributed workloads.

- **Lottery-Based Scheduling (LBS)**:
  - Allocates CPU time based on ticket distribution.
  - Processes with more tickets have higher chances of execution.
  - Features include:
    - **Random Ticket Selection**: Implements a pseudo-random generator for fair lottery.
    - **Inheritance of Tickets**: Child processes inherit tickets from their parent.
    - **Tie-Breaking**: Uses process arrival time to break ties, preventing starvation.

- **Multilevel Feedback Queue (MLFQ)**:
  - Prioritizes processes dynamically based on resource usage.
  - **Boosting**:
    - Resets all processes to the highest priority at fixed intervals.
  - **Aging**:
    - Gradually increases the priority of long-waiting processes.
  - **Behavior Adaptation**:
    - Lowers priority for CPU-bound tasks and raises it for I/O-bound tasks.
  - Provides optimal performance for mixed workloads by balancing fairness and responsiveness.

---

## License

This project is licensed under the MIT License. See the LICENSE file for details.

---
