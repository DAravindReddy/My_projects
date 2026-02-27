// check_command_type.c
#include "main.h"

extern char *internal_cmds[];


void getword(char *input, char *cmd) {
    int i = 0;
    while (input[i] != ' ' && input[i] != '\0') {
        cmd[i] = input[i];
        i++;
    }
    cmd[i] = '\0'; // Null-terminate
}

int check_command_type(char *input) {
    char cmd[100];  // FIX: missing variable
    getword(input, cmd);

    for (int i = 0; internal_cmds[i] != NULL; i++) {
        if (strcmp(cmd, internal_cmds[i]) == 0) {
            return INTERNAL;
        }
    }

    if (strncmp(cmd, "echo", 4) == 0) {
        return ECHO;
    }

    return EXTERNAL;
}


