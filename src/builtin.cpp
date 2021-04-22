#include <iostream>
#include <unistd.h>

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
        cerr << "cd: "<< args[1] <<": No such file or directory\n";
    }
    return 1;
}

int roosh_history(char **args)
{
    
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