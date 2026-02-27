#include "main.h"

void execute_piped_commands(int argc, char *argv[])
{
	int cmd_start[100];
    int cmd_count = 0;

    cmd_start[cmd_count++] = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "|") == 0) {
            argv[i] = NULL;
            cmd_start[cmd_count++] = i + 1;
        }
    }

    int prev_fd = -1;  // for previous pipe's read end

    for (int i = 0; i < cmd_count; i++) {
        int pipefd[2];

        // Create pipe only if not the last command
        if (i != cmd_count - 1) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            // Child process

            // If not first command: read input from previous pipe
            if (i != 0) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            // If not last command: write output to next pipe
            if (i != cmd_count - 1) {
                close(pipefd[0]); // close unused read end
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            // Execute the command
            execvp(argv[cmd_start[i]], &argv[cmd_start[i]]);

            // If command fails, only print for last command
            if (i == cmd_count - 1)
                perror("execvp failed");

            exit(EXIT_FAILURE);
        } else {
            // Parent process

            if (prev_fd != -1)
                close(prev_fd); // Close old read end

            if (i != cmd_count - 1) {
                close(pipefd[1]); // close write end in parent
                prev_fd = pipefd[0]; // save read end for next command
            }
        }
    }

    // Wait for all children
    for (int i = 0; i < cmd_count; i++) {
        wait(NULL);
    }
}


