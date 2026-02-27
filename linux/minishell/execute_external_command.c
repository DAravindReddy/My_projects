// execute_external_command.c
#include "main.h"

extern int exit_status;
int execute_external_command(char *input) {
    char *cmd[100];
    int i = 0;
    int bg = 0;

    size_t len = strlen(input);
    if (input[len - 1] == '&') {
        bg = 1;
        input[len - 1] = '\0';
    }

    char original_input[512];
    strncpy(original_input, input, sizeof(original_input));

    char *token = strtok(input, " ");
    while (token && i < 99) {
        cmd[i++] = token;
        token = strtok(NULL, " ");
    }
    cmd[i] = NULL;
    if (!cmd[0]) return FAILURE;

    pid_t ret = fork();
    if (ret < 0) {
        perror("fork");
        return FAILURE;
    }

    if (ret == 0) {
        setpgid(0, 0);
        if (execvp(cmd[0], cmd) == -1) {
            perror("execvp");
            exit(errno);
        }
    } else {
        if (bg) {
            printf("[%d] %d\n", job_count + 1, ret);
            job_list[job_count].pid = ret;
            strncpy(job_list[job_count].cmd, original_input, sizeof(job_list[job_count].cmd));
            job_list[job_count].is_running = 1;
            job_count++;
        } else {
            fg_pid = ret;
            int status;
            if (waitpid(ret, &status, WUNTRACED) == -1) {
                perror("waitpid");
                return FAILURE;
            }
            fg_pid = -1;

            if (WIFEXITED(status)) exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status)) {
                exit_status = 128 + WTERMSIG(status);
                printf("[%d] terminated by signal %d\n", ret, WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("[%d]+  Stopped                 %s\n", job_count + 1, original_input);
                exit_status = 128 + WSTOPSIG(status);
                job_list[job_count].pid = ret;
                strncpy(job_list[job_count].cmd, original_input, sizeof(job_list[job_count].cmd));
                job_list[job_count].is_running = 1;
                job_count++;
            }
        }
    }
    return SUCCESS;
}


