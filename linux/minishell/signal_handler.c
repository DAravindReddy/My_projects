#include "main.h"

void sigint_handler(int sig) {
    if (fg_pid > 0) {
        // Send SIGINT to the foreground process
        kill(fg_pid, SIGINT);
    } else {
        // Re-display the prompt if no foreground process
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, prompt, strlen(prompt));
        write(STDOUT_FILENO, " ", 1);
        fflush(stdout);
    }
}

void sigtstp_handler(int sig) {
    if (fg_pid > 0) {
        // Send SIGTSTP to the foreground process
        kill(fg_pid, SIGTSTP);

        // Print job stopped message
        printf("\n[%d]+  Stopped\t\tPID: %d\n", job_count + 1, fg_pid);

        // Add to job list
        job_list[job_count].pid = fg_pid;
        strcpy(job_list[job_count].cmd, "Background job");  // save actual command


        job_list[job_count].is_running = 0;
        job_count++;

        fg_pid = -1; // No more foreground process
    } else {
        // Re-display the prompt
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, prompt, strlen(prompt));
        write(STDOUT_FILENO, " ", 1);
        fflush(stdout);
    }
}


