// break the line into pieces and store in char **args

#pragma once

#include <string>
#include <vector>

std::pair<char **, int> roosh_parse(const std::string &line);
std::vector<std::string> roosh_tokenizer(const std::string &line, const char delimiter);