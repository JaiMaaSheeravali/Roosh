// run the command given by the user,
// either by exec system call
// or by builtin commands

#pragma once

#include <string>

bool roosh_launch(const std::string &line);