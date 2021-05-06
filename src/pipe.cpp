#include "../include/pipe.hpp"

#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <vector>

#include "../include/builtin.hpp"
#include "../include/parse.hpp"
#include "../include/redirect.hpp"

using namespace std;

int child = 0; /* whether it is a child process relative to main() */

// creating copy of stdin and
// stdout for restoring them later
int save_in = dup(STDIN_FILENO);
int save_out = dup(STDOUT_FILENO);

static void restore_io()
{
    dup2(save_in, STDIN_FILENO);
    dup2(save_out, STDOUT_FILENO);
}

static void report_error_and_exit(const char *msg)
{
    perror(msg);
    (child ? _Exit : exit)(EXIT_FAILURE);
}

// wrapper for close system call
static void Close(int close_fd)
{
    if (close(close_fd) == -1)
    {
        report_error_and_exit("close failed");
    }
}

/** move oldfd to newfd */
static void pipe_redirect(int oldfd, int newfd)
{
    if (oldfd != newfd)
    {
        if (dup2(oldfd, newfd) != -1)
            Close(oldfd); /* successfully redirected */
        else
            report_error_and_exit("dup2");
    }
}

// for executing last command in a long chain of commands
static bool pipe_exec_last(const string &cmd, int in_fd)
{
    /* redirect output to a file if asked */
    int out_fd = redirect_output(cmd);
    pipe_redirect(out_fd, STDOUT_FILENO);

    /* parent checks for builtin */
    int idx = check_builtin(cmd);
    if (idx != -1)
    {
        child = 0;
        pipe_redirect(in_fd, STDIN_FILENO); /* read from the read_end of old pipe/stdin */

        bool status = run_builtin(cmd, idx); /* run builtin-command */
        restore_io();                        /* restore stdin and stdout */
        return status;                       /* cmds execution complete */
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        report_error_and_exit("fork");
    }
    else if (pid == 0)
    {
        child = 1;
        pipe_redirect(in_fd, STDIN_FILENO); /* read from in_fd */

        auto [args, num_args] = roosh_parse(cmd);
        execvp(args[0], args);
        report_error_and_exit("execvp");
    }

    // close the old pipe
    if (in_fd != STDIN_FILENO)
    {
        Close(in_fd);
    }

    wait(NULL);
    restore_io();
    return 1;
}

bool pipe_exec(const vector<string> &cmds, size_t pos, int in_fd)
{
    string cmd = cmds[pos];

    if (pos == cmds.size() - 1) /* last command */
    {
        return pipe_exec_last(cmd, in_fd);
    }
    else
    {
        /* $ <in_fd cmds[pos] >fd[1] | <fd[0] cmds[pos+1] ... */
        int fd[2]; /* output pipe */
        if (pipe(fd) == -1)
        {
            report_error_and_exit("pipe");
        }

        // parent checks for builtin
        int idx = check_builtin(cmd);

        // if it is a builtin command, parent runs it
        // and moves on to the next command without forking
        if (idx != -1)
        {
            child = 0;
            pipe_redirect(in_fd, STDIN_FILENO);  /* read from the read_end of old pipe/stdin */
            pipe_redirect(fd[1], STDOUT_FILENO); /* write to the write_end of new pipe */

            run_builtin(cmd, idx);                  /* run builtin-command */
            restore_io();                           /* restore stdin and stdout */
            return pipe_exec(cmds, pos + 1, fd[0]); /* execute the rest */
        }

        // else fork and exec the command
        pid_t pid = fork();

        if (pid < 0)
        {
            report_error_and_exit("fork");
        }
        else if (pid == 0)
        {
            child = 1;
            Close(fd[0]);                        /* unused */
            
            pipe_redirect(in_fd, STDIN_FILENO);  /* read from in_fd */
            pipe_redirect(fd[1], STDOUT_FILENO); /* write to fd[1] */

            auto [args, num_args] = roosh_parse(cmd);
            execvp(args[0], args);
            report_error_and_exit("execvp");
        }

        // parent process
        Close(fd[1]); /* unused */
        if (in_fd != STDIN_FILENO)
        {
            Close(in_fd); /* unused */
        }

        wait(NULL);
        return pipe_exec(cmds, pos + 1, fd[0]); /* execute the rest */
    }
}