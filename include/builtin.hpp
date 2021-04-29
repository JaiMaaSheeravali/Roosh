#pragma once
#include <string>

int roosh_cd(char **args);
int roosh_history(char **args);
int roosh_exit(char **args);
int roosh_rsh(char **args);

void push_command(std::string cmd);