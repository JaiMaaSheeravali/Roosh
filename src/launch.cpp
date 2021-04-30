#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/builtin.hpp"

using namespace std;

const char *builtin_list[] = {
    "cd",
    "history",
    "exit",
    "roosh"};

int (*builtin_func[])(char **, int) = {
    &roosh_cd,
    &roosh_history,
    &roosh_exit,
    &roosh_rsh};

bool roosh_exec(char **args)
{
    pid_t pid = 0;
    pid = fork();

    if (pid < 0)
    {
        cerr << "fork failed\n";
    }
    else if (pid == 0)
    {
        // child process
        if (execvp(args[0], args) == -1)
        {
            // kill the child process if an
            // error occurs
            cerr << "no such command exists\n";
            _Exit(EXIT_FAILURE);
        }
    }

    // parent process
    wait(NULL);
    return 1;
}

bool roosh_launch(char **args, int num_args)
{
    if (args[0] == NULL)
    {
        return 1;
    }

    // built-in command
    size_t builtin_len = sizeof(builtin_list) / sizeof(char *);
    for (size_t i = 0; i < builtin_len; i++)
    {
        if (strcmp(builtin_list[i], args[0]) == 0)
        {
            return (*builtin_func[i])(args, num_args);
        }
    }

    // linux kernel command
    return roosh_exec(args);
}