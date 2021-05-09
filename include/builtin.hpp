#pragma once
#include <string>
#include <vector>

const std::vector<std::string> builtin_list{
    "cd",
    "history",
    "exit",
    "roosh",
    "google",
    "tutorial"};

int check_builtin(const std::string &cmd);
bool run_builtin(const std::string &cmd, int idx);
int roosh_cd(char **args, int num_args);
int roosh_history(char **args, int num_args);
int roosh_exit(char **args, int num_args);
int roosh_rsh(char **args, int num_args);
int roosh_google(char **args, int num_args);

void push_command(std::string cmd);