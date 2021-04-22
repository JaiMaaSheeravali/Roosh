#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/builtin.hpp"

using namespace std;

const char *builtin_list[] = {
    "cd",
    "history",
    "exit"};

int (*builtin_func[])(char **) = {
    &roosh_cd,
    &roosh_history,
    &roosh_exit};

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
            cerr << "no such command exists\n";
        }
    }

    // parent process
    wait(NULL);
    return 1;
}

bool roosh_launch(char **args, int n)
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
            return (*builtin_func[i])(args);
        }
    }

    // linux kernel command
    return roosh_exec(args);
}