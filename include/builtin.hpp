#pragma once
#include <string>

int roosh_cd(char **args, int num_args);
int roosh_history(char **args, int num_args);
int roosh_exit(char **args, int num_args);
int roosh_rsh(char **args, int num_args);

void push_command(std::string cmd);