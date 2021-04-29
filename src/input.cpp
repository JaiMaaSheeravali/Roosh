#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <string.h>
#include <iostream>

#include <../include/color.hpp>

using namespace std;

void print_git_branch();

void print_input_format()
{
    cout << flush;

    char hostname[50];
    struct passwd *p = getpwuid(getuid());

    // Get the host name and user name of current user
    if (gethostname(hostname, sizeof(hostname)) == 0 && p)
    {
        cout << BLUE << p->pw_name << "@";
        cout << hostname << RESET;
    }

    // Print the current working directory of user
    char cwd[100];

    cout << GREEN;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        cout << cwd;
    else
        cout << ".";
    cout << RESET;

    // Get git branch if current directory is a git directory
    print_git_branch();
    cout << "₹ " << flush;
}

void print_git_branch()
{
    FILE *fp;
    char branch_name[400];

    // Get current branch and save its name in a file.
    fp = popen("git branch --show-current 2>/dev/null", "r");

    if (fp == NULL){
        pclose(fp);
        return;
    }

    // Read branch name and print current branch name
    fgets(branch_name, 200, fp);
    branch_name[strcspn(branch_name, "\n")] = 0;

    // Should check status if any error occurs
    pclose(fp);

    if (branch_name && strcmp(branch_name, "") == 0)
        return;

    // print current branch name in red enclosed in round braces
    cout << RED << " (" << branch_name << ")" << RESET;
}