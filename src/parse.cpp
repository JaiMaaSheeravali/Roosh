#include "../include/parse.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

std::pair<char **, int> roosh_parse(const string &line)
{
    // Used to split string around spaces.
    istringstream iss(line);

    string word; // for storing each word

    size_t n = 0; // no of words (arguments)
    vector<string> vs;
    while (iss >> word)
    {
        if (word == "<" || word == ">" || word == ">>")
        {
            break;
        }

        vs.push_back(word);
        n++;
    }

    char **args = new char *[n + 1];
    for (size_t i = 0; i < n; i++)
    {
        word = vs[i];
        args[i] = new char[word.size() + 1];
        strcpy(args[i], word.c_str());
    }

    // null needed for execvp to function properly
    args[n] = NULL;

    return {args, n};
}

// trim from start
static inline std::string &ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace)))
                .base(),
            s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}

// return list of parameters of a command
vector<string> roosh_tokenizer(const string &line, const char delimiter)
{
    vector<string> tokens;
    string token;            // a single token
    istringstream iss(line); // used to split string around delimiter

    // Tokenizing w.r.t. specified delimiter
    while (getline(iss, token, delimiter))
    {
        token = trim(token);
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}