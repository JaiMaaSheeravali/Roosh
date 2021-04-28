#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <fstream>

#include "../include/parse.hpp"
#include "../include/launch.hpp"

using namespace std;
int roosh_cd(char **args)
{
    // number of arguments must be exactly two
    // first one is "cd"
    // second is directory path
    if (args[1] == NULL)
    {
        cerr << "cd: unexpected number of arguments\n";
        return 1;
    }

    // if chdir failed print the error_message
    if (chdir(args[1]) == -1)
    {
        cerr << "cd: " << args[1] << ": No such file or directory\n";
    }
    return 1;
}

// this will take a stream and run all
// commands in that stream in batch mode
void roosh_batch_loop(std::istream &in)
{
    string line;

    // keep taking the input till EOF or exit
    while (getline(in, line))
    {

        // empty command i.e 'enter key'
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        // exit through batch mode isn't allowed
        auto [args, num_args] = roosh_parse(line);
        roosh_launch(args, num_args);

        // deallocating memory
        for (int i = 0; i < num_args; i++)
        {
            delete[] args[i];
        }
        delete[] args;
    }
}

int roosh_rsh(char **args)
{

    // Produce error if no file is specified.
    if (args[1] == NULL)
    {
        cerr << "Batch Mode: No file specified to run in Batch Mode\n";
        return 1;
    }

    // Check if extension of file is correct or not
    int slen = strlen(args[1]);
    char buf[5];
    strncpy(buf, args[1] + slen - 4, 4);

    if (strcmp(buf, ".rsh") == 0)
    {

        // Make a file stream from file and call roosh batch loop
        std::ifstream infile(args[1]);

        roosh_batch_loop(infile);
    }
    else
    {
        cerr << "Invalid File Format! Not a Batch File.\n";
    }
    return 1;
}

int roosh_history(char **args)
{
    return 1;
}
int roosh_exit(char **args)
{
    // only one arg should be given
    if (args[1] != NULL)
    {
        return 1;
    }

    return 0;
}