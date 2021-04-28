// break the line into pieces and store in char **args

#pragma once

#include <string>
std::pair<char **, int> roosh_parse(const std::string &line);
