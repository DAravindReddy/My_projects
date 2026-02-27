// internal_command.c

#include "main.h"

char *internal_cmds[] = { "cd", "pwd", "jobs", "fg", "bg", NULL };

int execute_internal_command(char *input) {
    if (strncmp(input, "cd", 2) == 0) {
        char *path = input + 3;
        if (chdir(path) != 0) perror("cd");
        return SUCCESS;
    }

    if (strcmp(input, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) printf("%s\n", cwd);
        else perror("pwd");
        return SUCCESS;
    }

    if (strcmp(input, "jobs") == 0) {
        int found = 0;
        for (int i = 0; i < job_count; i++) {
            if (job_list[i].is_running) {
                found = 1;
                printf("[%d] Running\t\t%s [%d]\n", i + 1, job_list[i].cmd, job_list[i].pid);
            }
        }
        if (!found) printf("No background jobs\n");
        return SUCCESS;
    }

    if (strncmp(input, "fg", 2) == 0) {
        int job_num = atoi(input + 3) - 1;
        if (job_num >= 0 && job_num < job_count) {
            printf("Bringing [%d] %s to foreground\n", job_num + 1, job_list[job_num].cmd);
            kill(job_list[job_num].pid, SIGCONT);
            fg_pid = job_list[job_num].pid;
            int status;
            waitpid(job_list[job_num].pid, &status, WUNTRACED);
            fg_pid = -1;
            if (WIFSTOPPED(status)) job_list[job_num].is_running = 1;
            else job_list[job_num].is_running = 0;
        } else {
            printf("fg: invalid job number\n");
        }
        return SUCCESS;
    }

    if (strncmp(input, "bg", 2) == 0) {
        int job_num = atoi(input + 3) - 1;
        if (job_num >= 0 && job_num < job_count) {
            printf("Resuming [%d] %s in background\n", job_num + 1, job_list[job_num].cmd);
            kill(job_list[job_num].pid, SIGCONT);
            job_list[job_num].is_running = 1;
        } else {
            printf("bg: invalid job number\n");
        }
        return SUCCESS;
    }

    return FAILURE;
}


