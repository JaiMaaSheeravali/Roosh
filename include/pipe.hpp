#pragma once

#include <string>
#include <vector>

bool pipe_exec(const std::vector<std::string> &cmds, size_t pos, int in_fd);