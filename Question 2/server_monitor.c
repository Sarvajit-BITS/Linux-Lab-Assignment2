/* =====================================================================
 * File Name: server_monitor.c
 * Author: Sarvajit Parsaniya
 * Purpose: Solve the web server unresponsiveness problem.
 *          Creates child processes, monitors execution time, 
 *          terminates hung processes, and prevents zombie accumulation.
 * ===================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define NUM_CHILDREN 3
#define TIMEOUT_LIMIT 5  /* I set the maximum allowed execution time to 5 seconds */

int main() {
    pid_t child_pids[NUM_CHILDREN];
    time_t start_times[NUM_CHILDREN];
    int active_children = NUM_CHILDREN;

    printf("Parent Process: Starting web server monitor. My PID is %d\n", getpid());

    /* Step 1: Process Creation using fork() */
    for (int i = 0; i < NUM_CHILDREN; i++) {
        /* I call fork() to duplicate my parent process and create a child */
        child_pids[i] = fork();

        if (child_pids[i] < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (child_pids[i] == 0) {
            /* This code block runs only inside my child processes */
            int child_number = i + 1;
            printf("Child %d: Created successfully with PID %d\n", child_number, getpid());

            if (child_number == 3) {
                /* I make Child 3 simulate an unresponsive hung web process */
                printf("Child %d (PID %d): Simulating a hung state (infinite loop)...\n", child_number, getpid());
                while (1) {
                    sleep(1); /* I sleep repeatedly to simulate a hanging loop */
                }
            } else {
                /* I make Child 1 and 2 simulate normal, fast-completing request processes */
                int work_duration = child_number * 2; /* Child 1 runs 2s, Child 2 runs 4s */
                printf("Child %d (PID %d): Processing web requests for %d seconds...\n", 
                       child_number, getpid(), work_duration);
                sleep(work_duration);
                printf("Child %d (PID %d): Finished request. Exiting normally.\n", child_number, getpid());
                exit(EXIT_SUCCESS); /* Child terminates normally */
            }
        } else {
            /* This code block runs only inside my parent process */
            /* I record the exact timestamp when this child process was spawned */
            start_times[i] = time(NULL);
        }
    }

    printf("Parent Process: Now actively monitoring %d child processes...\n", NUM_CHILDREN);

    /* Step 2: Active Monitoring and Zombie Prevention Loop */
    while (active_children > 0) {
        sleep(1); /* I wait 1 second between each audit scan to avoid hogging the CPU */
        time_t current_time = time(NULL);

        for (int i = 0; i < NUM_CHILDREN; i++) {
            /* I skip children that I have already successfully reaped and cleared */
            if (child_pids[i] == 0) {
                continue;
            }

            int status;
            /* I use waitpid() with the WNOHANG option to check status without blocking */
            pid_t wait_result = waitpid(child_pids[i], &status, WNOHANG);

            if (wait_result == child_pids[i]) {
                /* I observe that the child has terminated on its own */
                printf("Parent Process: Reaped Child %d (PID %d) successfully. No zombie created.\n", 
                       i + 1, child_pids[i]);
                child_pids[i] = 0; /* I clear the PID slot to stop tracking it */
                active_children--;
            } else if (wait_result == 0) {
                /* I observe that the child is still running. I calculate its execution time */
                double elapsed_seconds = difftime(current_time, start_times[i]);

                if (elapsed_seconds >= TIMEOUT_LIMIT) {
                    printf("Parent Process: Child %d (PID %d) has exceeded timeout (running for %.0f seconds)!\n", 
                           i + 1, child_pids[i], elapsed_seconds);
                    printf("Parent Process: Sending SIGKILL to terminate unresponsive child...\n");

                    /* Step 3: Signaling the unresponsive child */
                    if (kill(child_pids[i], SIGKILL) == -1) {
                        perror("kill signal failed");
                    } else {
                        /* After sending SIGKILL, I must wait on it to prevent it from becoming a zombie */
                        waitpid(child_pids[i], &status, 0); 
                        printf("Parent Process: Reaped Child %d (PID %d) after forced signal termination.\n", 
                               i + 1, child_pids[i]);
                        child_pids[i] = 0; /* I clear the PID slot */
                        active_children--;
                    }
                }
            } else if (wait_result == -1) {
                perror("waitpid error occurred");
            }
        }
    }

    printf("Parent Process: All child processes have been safely managed. Monitor exiting cleanly.\n");
    return 0;
}