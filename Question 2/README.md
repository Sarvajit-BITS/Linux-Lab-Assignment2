## My Lab Report: Question 2 Solution

I have designed, written, and tested a C program that solves the unresponsive web server problem. It uses fork() to create child processes, actively monitors their running time to detect unresponsive hangs, terminates hung processes using SIGKILL, and reaps all completed children using a non-blocking waitpid() loop to prevent zombie processes.

Below are the exact terminal commands I ran to compile and test the program, my observations, and my conceptual explanations of how these components work together. The documented C source code has been uploaded as a separate file in this repository.

# =======================================================
## 1. Lab Setup and Execution Log

These are the exact steps and shell commands I used to write, transfer, compile, and execute my program.

### Step 1: Write and Transfer the Source Code
**Observation:** I used Visual Studio Code (VS Code) to write my `server_monitor.c` file, including detailed comments to explain my logic. Once the code was ready, I moved the file into my Coursera Lab environment to prepare for compilation.

### Step 2: I compiled my C program
gcc server_monitor.c -o server_monitor

**My Explanation and Observation:** I ran the gcc compiler command to translate my human-readable C file into machine code. I observed that the command ran cleanly without displaying any compiler errors or warning blocks, outputting the executable server_monitor.

### Step 3: I executed my process monitor
./server_monitor

**My Explanation and Observation:** I executed my compiled program directly from my active terminal directory. I observed the program start up, spawn 3 child processes, successfully wait and reap Child 1 (at 2s) and Child 2 (at 4s) when they exited, detect Child 3 as hung at 5s, kill Child 3 with SIGKILL, and exit cleanly.

#=========================================================
## 2: How Process Creation, Waiting, and Signals Work Together

Based on my computer science studies and sources, I can explain how these three operating system mechanisms cooperate to solve the problem of unresponsive web servers:
**Process Creation (fork):** My web server uses the fork() system call to delegate work. The kernel creates a child process by making an almost exact duplicate of the parent's address space, stack, data, and heap segments
. Because of process isolation, if a child crashes or hangs while parsing bad network input, it does not crash my main parent program
. The parent receives the child's unique Process ID (PID) as a return value, allowing it to track that specific process

**State Monitoring and Zombie Prevention (waitpid):** When a child process terminates, the kernel frees its physical memory but leaves its process table entry intact, storing its PID and termination status; this temporary state is called a zombie process
. If I do not clean these up, they will clog my server's process table and crash the operating system
. By calling waitpid() with the WNOHANG flag, my parent process can perform a non-blocking check on each child
. This allows my parent to continue running its monitoring loop instead of freezing on a single hung process

**Signal Handling and Forced Termination (kill):** Signals are the standard UNIX mechanism for inter-process communication
. When my parent process detects that a child has exceeded its 5-second execution limit, it uses the kill() system call to send a signal
. I selected SIGKILL (signal number 9) because it is a special-purpose signal that is handled directly by the kernel; it cannot be caught, ignored, or blocked by the misbehaving child, guaranteeing immediate termination of my unresponsive process

**Synthesis:** Process creation yields a trackable PID; non-blocking waiting checks if that PID is running without stalling my server; and signals let my parent step in to eliminate the child if it exceeds my safety thresholds. Once the child is signaled, my parent immediately reaps it to keep the system's process table completely clean.