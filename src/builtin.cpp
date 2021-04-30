#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sys/types.h>
#include <pwd.h>

#include "../include/parse.hpp"
#include "../include/launch.hpp"
using namespace std;

// vector to store list of commands
vector<string> list_cmds;

// Function to print error message when
void invalid_arg_count_error(int num_args, int required_args){
    cerr << "Error: Invalid number of Arguments(" << num_args-1 << " given) - " << required_args << " Required!\n";
}

int roosh_cd(char **args, int num_args)
{
    // number of arguments must be exactly two
    // first one is "cd"
    // second is directory path
    if (num_args == 1)
    {
        cerr << "Error cd: Target directory address not given\n";
        return 1;
    }

    // Display error if extra arguments are given
    if(num_args > 2){
        invalid_arg_count_error(num_args, 1);
        return 1;
    }

    // change args[1] by home directory location if "cd ~" is used
    if(strcmp(args[1], "~") == 0){

        struct passwd *pw = getpwuid(getuid());
        args[1] = strdup(pw->pw_dir);
    }

    // if chdir failed print the error_message
    if (chdir(args[1]) == -1)
    {
        cerr << "Error! " << args[1] << ": No such directory exits\n";
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

int roosh_rsh(char **args, int num_args)
{

    // Produce error if no file is specified.
    if (num_args == 1)
    {
        cerr << "Batch Mode: No file specified to run in Batch Mode\n";
        return 1;
    }

    // Display error if extra arguments are given
    if(num_args > 2){
        invalid_arg_count_error(num_args, 1);
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

// roosh_history implementation

// used by the main program to push
// a new commmand into the list of
// commands ran so far
void push_command(string line)
{
    // remove whitespaces from the command
    string cmd;

    // Used to split string around spaces.
    istringstream iss(line);

    string word; // for storing each word

    while (iss >> word)
    {
        cmd += word;
        cmd += ' ';
    }

    list_cmds.push_back(cmd);
}

// prints the commands ran so far
int roosh_history(char **args, int num_args)
{
    // only one argument i.e history
    // is expected
    if (num_args > 1)
    {
        invalid_arg_count_error(num_args, 0);
        return 1;
    }

    int id = 1;
    for (auto cmd : list_cmds)
    {
        cout << "  " << std::left << setw(4) << id << " ";
        cout << cmd << endl;
        id++;
    }

    return 1;
}

int roosh_exit(char **args, int num_args)
{
    // only one arg should be given
    if (num_args > 1)
    {
        invalid_arg_count_error(num_args, 0);
        return 1;
    }

    return 0;
}