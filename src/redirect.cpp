#include "../include/redirect.hpp"

#include <sys/fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "../include/parse.hpp"

using namespace std;

int redirect_input(const string &cmd)
{
    // if no '<' symbol is found
    // then take the input from stdin
    if (cmd.find('<') == std::string::npos)
    {
        return STDIN_FILENO;
    }

    auto tokens = roosh_tokenizer(cmd, '<');  // split whole command around '<'
    tokens = roosh_tokenizer(tokens[1], '>'); // split remaining command around '>' to get input file name

    int in_fd = open(tokens[0].c_str(), O_RDONLY); // opening the file in read-only mode
    if (in_fd == -1)
    {
        perror("redir_input");
    }

    return in_fd;
}

int redirect_output(const string &cmd)
{

    // if no '>' or '>>' symbol is found
    // then send the output to stodut
    if (cmd.find('>') == std::string::npos)
    {
        return STDOUT_FILENO;
    }

    string delimiter = ">";
    if (cmd.find(">>") != std::string::npos)
    {
        delimiter = ">>";
    }

    auto tokens = roosh_tokenizer(cmd, '>'); // split whole command around '>'

    int out_fd; // output: file descriptor

    // append mode
    if (delimiter == ">>")
    {
        out_fd = open(tokens[1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
    }
    // overwrite mode
    else
    {
        out_fd = open(tokens[1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }

    // opening the file in read-only mode
    if (out_fd == -1)
    {
        perror("redir_output");
        _Exit(EXIT_FAILURE);
    }

    return out_fd;
}