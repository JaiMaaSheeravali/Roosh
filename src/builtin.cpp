#include "../include/builtin.hpp"

#include <pwd.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tinyexpr.h>
#include "../include/parse.hpp"
#include "../include/launch.hpp"
#include "../include/google.hpp"
#include "../include/tutorial.hpp"

using namespace std;

// function pointers to builtin functions
int (*builtin_func[])(char **, int) = {
    &roosh_cd,
    &roosh_history,
    &roosh_exit,
    &roosh_rsh,
    &roosh_google,
    &roosh_exec_tutorial,
    &roosh_calc};

// vector to store history of list of commands
vector<string> list_cmds;

// Function to print error message when
void invalid_arg_count_error(int num_args, int required_args)
{
    cerr << "Error: Invalid number of Arguments(" << num_args - 1 << " given) - " << required_args << " Required!\n";
}

int check_builtin(const std::string &cmd)
{
    for (size_t i = 0; i < builtin_list.size(); i++)
    {
        if (cmd.substr(0, cmd.find(" ")) == builtin_list[i])
        {
            return i; // return index of which builtin command to be run
        }
    }

    // return -1 if command is not a built-in command
    return -1;
}

bool run_builtin(const string &cmd, int idx)
{
    auto [args, num_args] = roosh_parse(cmd);

    // run the idx'th command from bulitin_list
    bool status = (*builtin_func[idx])(args, num_args);

    // deallocating memory
    for (int i = 0; i < num_args; i++)
    {
        delete[] args[i];
    }
    delete[] args;

    return status;
}

/*** roosh change directory implementation ***/

int roosh_cd(char **args, int num_args)
{

    // Display error if extra arguments are given
    if (num_args > 2)
    {
        invalid_arg_count_error(num_args, 1);
        return 1;
    }

    // change args[1] by home directory location if "cd ~" is used
    if (num_args == 1 || strcmp(args[1], "~") == 0)
    {
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

/*** roosh scripts.rsh implementation ***/

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
        roosh_launch(line);
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
    if (num_args > 2)
    {
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

/*** roosh history implementation ***/

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

/*** roosh google search implementation ***/

// function to print two strings in two columns
static void print_two_cols(const string &s1, const string &s2, int width)
{
    cout << "  " << std::left << setw(width) << s1 << " ";
    cout << s2 << endl;
}

// function to print google help message
static void print_google_help()
{
    cout << "  Format:\n"
         << "  google <word_to_search>  : In this default language is English US.\n"
         << "  google <word_to_search> <lang_code> : To search in specific language.\n\n"
         << "  Example:\n"
         << "  google hello\n"
         << "  google hello US\n\n";

    print_two_cols("--------------", "---------", 20);
    print_two_cols("Language Code", "Language", 20);
    print_two_cols("--------------", "---------", 20);
    print_two_cols(" US", "English (US)", 20);
    print_two_cols(" UK", "English (UK)", 20);
    print_two_cols(" hi", "Hindi", 20);
    print_two_cols(" es", "Spanish", 20);
    print_two_cols(" fr", "French", 20);
    print_two_cols(" ja", "Japanese", 20);
    print_two_cols(" ru", "Russian", 20);
    print_two_cols(" de", "German", 20);
    print_two_cols(" it", "Italian", 20);
    print_two_cols(" ko", "Korean", 20);
    print_two_cols(" ar", "Arabic", 20);
    print_two_cols(" tr", "Turkish", 20);
}

int roosh_google(char **args, int num_args)
{
    if (num_args == 1)
    {
        cout << "Please use google --help for any help.\n";
    }
    else if (num_args == 2)
    {
        if (!strcmp(args[1], "--help"))
        {
            print_google_help();
        }
        else
        {
            google_a_description(args[1], "en_US");
        }
    }
    else if (num_args == 3)
    {
        if (!strcmp(args[2], "US"))
            google_a_description(args[1], "en_US");
        else if (!strcmp(args[2], "UK"))
            google_a_description(args[1], "en_UK");
        else
            google_a_description(args[1], args[2]);
    }
    else
    {
        cout << "google: error invalid number of arguments\n";
        cout << "Please use google --help for any help.\n";
    }

    return 1;
}

/*** roosh calculator implementation ***/

int roosh_calc(char **args, int num_args)
{
    // 2 arguments required
    // first is the calc command
    // second is the expresssiongi
    if (num_args > 2)
    {
        invalid_arg_count_error(num_args, 1);
        return 1;
    }
    if (num_args == 1 || !strcmp(args[1], "--help"))
    {
        cout << "\nFormat:\tcalc <expression> \n\n";
        cout << "\t+\tAddition\n\t-\tSubtraction\n\t*\tMultiplication\n\t/\tDivide\n\t^\tExponent\n\t%\tModulus\n\t()\tBrackets\n\n";

        return 1;
    }

    // tinyexpr evaluates the expresssion

    double answer = te_interp(args[1], 0);
    cout << answer << endl;

    return 1;
}
