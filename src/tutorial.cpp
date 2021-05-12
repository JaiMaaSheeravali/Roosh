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
#include <../include/color.hpp>
#include <fstream>
#include <iomanip>

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
    // roosh_launch("rm -rf " + home_dir + "/tutorial");
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


bool write_tutorial(string filepath)
{
    string line;
    filepath = home_dir+"/tutorial/tutorial_cmd/"+filepath;

    ifstream myfile(filepath);

    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            string cmd_path = home_dir + "/tutorial/tutorial_cmd/" + line + ".txt";
            ifstream filename(cmd_path);
            cout << MAGENTA;
            cout << "\t" << line << "\t\t"; //60
            cout << RESET;
            getline(myfile, line);
            cout << line << endl; //88
            cout << endl;
            cout << endl;
            string cmd;

            if (filename.is_open())
            {

                while (getline(filename, cmd))
                {
                    cout << CYAN;
                    cout << "\t" << cmd << endl;
                    getline(filename, cmd);
                    cout << RESET;
                    cout << "\t-> " << cmd << endl;
                    cout << endl;
                }

                filename.close();
            }
            int n = 50;
            while (n--)
                cout << "-";
            cout << endl;
            cout << endl;

            cout << "Please Enter 'q' to exit or any other key to continue: ";

            string user_choice;
            getline(cin, user_choice);

            cout << endl;
            
            if(user_choice == "q"){
                myfile.close();
                return true;
            }
        }
        myfile.close();
    }

    else
        cout << "Unable to open file";
    
    return false;
}

int roosh_tutorial()
{
    bool exit_tut = false;

    cout << YELLOW << "\t\tWelcome to Roosh Tutorial!!!" << RESET << endl;
    cout << endl;
    cout << "\tOur Tutorial only provides a basic usage overview of most commonly used commands in linux." << endl;
    cout << "\tTo know more about any command you can always use " << RED << "'man command'" << RESET << " in Roosh Input prompt." << endl;
    cout << endl;
    cout << "We have also provided certain test levels to test your understanding of tutorial content." << endl;
    cout << "Levels can be accessed by using " << RED << "'tutorial level [level-number]'" << RESET << " command." << endl;
    cout << "You have to find password hidden inside level and submit it using: \n\t" << RED << "'tutorial level [level-number] password'";
    cout << RESET << " command and enterning the password." << endl;
    cout << endl;
    cout << endl;

    cout << "Enter any key to continue or q to exit: ";

    string user_choice;
    getline(cin, user_choice);

    if(user_choice == "q")
        return 1;

    cout << endl;    

    cout << "\n"
         << BLUE << "          "
         << "Basic linux commands" << endl;
    cout << RESET << endl;
    exit_tut = write_tutorial("basiccmdlist.txt");

    if(exit_tut)
        return 1;

    cout << BLUE << "            "
         << "Git linux commands" << endl;
    cout << RESET << endl;
    exit_tut = write_tutorial("gitcmdlist.txt");
    
    return 1;
}