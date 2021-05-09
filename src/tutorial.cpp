#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <pwd.h>
#include "../include/parse.hpp"
#include "../include/launch.hpp"
#include "../include/levels.hpp"
#include "../include/tutorial.hpp"

using namespace std;

string home_dir;
extern bool unlocked[10];
extern int level;
int roosh_tutorial();

int create_tutorial_folder()
{
    char *home;
    struct passwd *pw = getpwuid(getuid());
    home = strdup(pw->pw_dir);
    home_dir = home;
    roosh_launch("cp -r tutorial " + home_dir);
    return 0;
}

int roosh_exec_tutorial(char **args, int num_args)
{
    //number of arguments can be =>
    // 1 i.e. tutorial
    // 3 i.e. tutorial level <1-10>
    // 4 i.e. tutorial level <1-10> password

    //tutorial cmd
    if (num_args == 1)
    {

        return roosh_tutorial();
    }

    // case of 3 or 4 arguments
    else if ((num_args == 3 || num_args == 4) && (strcmp(args[1], "level") == 0))
    {
        if (num_args == 4)
        { //error if fourth arg is not password
            if ((strcmp(args[3], "password") != 0))
            {
                cerr << "no such command exists\n";
                return 1;
            }
        }

        //parse the level value to int
        int level;
        stringstream ss;
        ss << args[2];
        ss >> level;

        //level must be b/w 1-10
        if (level > 0 && level <= 10 && (strlen(args[2]) == level / 10 + 1))
        {

            if (num_args == 3)
            {
                //display corresponding instructins
                string path = (home_dir + "/tutorial/lvl_instrns/level" + args[2] + ".txt");
                print_instructions(path);

                //navigate to corresponding directory
                path = (home_dir + "/tutorial/tutorial_levels/level" + args[2]);
                chdir(path.c_str());
            }

            else
            { // for level 1 just check the password entered
                if (level == 1)
                {
                    check_password(level);
                    return 1;
                }
                //else first check whether the level is unlocked or not
                else if (unlocked[level - 1])
                {
                    check_password(level);
                    return 1;
                }

                else
                {
                    print_no_access_to_level();
                }
            }
            return 1;
        }

        else
        {
            //display error if level is not an integer bw 1 nd 10
            cerr << "Please enter a level between 1 to 10.\n";
            return 1;
        }
    }

    else
    {
        cerr << "no such command exists\n";
        return 1;
    }
}

int roosh_tutorial()
{
    cout << "My tutorial :)\n";
    return 1;
}
