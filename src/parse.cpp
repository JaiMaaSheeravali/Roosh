#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <utility>

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