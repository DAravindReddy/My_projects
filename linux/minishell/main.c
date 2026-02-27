
 /*Requirements
  * 1. Customized prompt
  * 2. special variables
  * 3. Internal command
  * 4. External command
  * 5. built in functions
  */

#include "main.h"

#define MAX_ARGS 100

// Define the variables here
Job job_list[100];   // actual memory allocation
int job_count = 0;   // actual definition

pid_t fg_pid = -1;

int exit_status;

char prompt[50] = "minishell$";

int echo(char *input)
{
	if(!strcmp(input + 5, "$?"))
	{
		printf("%d\n", exit_status);
	}
	else if(!strcmp(input + 5, "$$"))
	{
		printf("%d\n", getpid());
	}
	else if(!strcmp(input + 5, "$SHELL"))
	{
		char *cwd = get_current_dir_name(); // Dynamically allocates memory
    		if (cwd != NULL)
    		{
        		printf("%s/\n", cwd);

        		free(cwd); // Free the memory allocated by get_current_dir_name
    		}
    		else
    		{
        		perror("get_current_dir_name");
    		}

	}
	else
	{
		printf("%s\n", input + 5);
	}
}


int main()
{
    system("clear");
    char input[500];
    int ret;

    // Setup sigaction for SIGINT
    struct sigaction sa, sa1;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);   // no blocked signals during handler
    sa.sa_flags = SA_RESTART;   // restart interrupted syscalls like fgets

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa1.sa_handler = sigtstp_handler;
    sigemptyset(&sa1.sa_mask);
    sa1.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa1, NULL);

    while (1)
    {
        printf("%s ", prompt);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;  // handle Ctrl+C interruptions safely
        }

        input[strlen(input) - 1] = '\0';

	 if (strchr(input, '|') != NULL) {
            // Split the input string into argv format
            char *args[MAX_ARGS];
            int argc = 0;

            char *token = strtok(input, " ");
            while (token && argc < MAX_ARGS - 1) {
                args[argc++] = token;
                token = strtok(NULL, " ");
            }
            args[argc] = NULL;

            // Call the pipe executor
           execute_piped_commands(argc, args);
	   continue;
	}
        if (!strcmp(input, "exit"))
            exit(0);
        else if (!strncmp(input, "PS1=", 4))
            strcpy(prompt, input + 4);
        else if ((ret = check_command_type(input)) == EXTERNAL)
            execute_external_command(input);
        else if (ret == INTERNAL)
            execute_internal_command(input);
        else if (ret == ECHO)
            echo(input);
        else
            printf("%s: command not found\n", input);
    }

    return 0;
}

