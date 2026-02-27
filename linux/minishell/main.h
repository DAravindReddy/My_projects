// main.h
#ifndef MAIN_H
#define MAIN_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

#include <signal.h>
#include <sys/types.h>

// status macros
#define SUCCESS   0
#define FAILURE  -1
#define EXTERNAL  1
#define INTERNAL  2
#define ECHO      3

#define MAX_JOBS 100

typedef struct job {
    pid_t pid;
    char cmd[256];
    int is_running;
} Job;


// Global declarations (not definitions)
extern Job job_list[100];
extern int job_count;

extern pid_t fg_pid;
extern char prompt[];

// Prototypes
int execute_external_command(char *input); // execute cmds like ls, wc, grep, sleep, cat, ps, mkdir etc 

int execute_internal_command(char *input);  //execute cmds like exit, cd, echo family, PS1=, jobs, fg, bg

int check_command_type(char *input);

void sigint_handler(int sig);

void sigtstp_handler(int sig);

void execute_piped_commands(int argc, char *argv[]);// Declare function





#endif




