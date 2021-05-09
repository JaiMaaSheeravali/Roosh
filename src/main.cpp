#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <csignal>
#include "../include/loop.hpp"
#include "../include/parse.hpp"
#include "../include/launch.hpp"
#include "../include/input.hpp"
#include "../include/builtin.hpp"
#include "../include/tutorial.hpp"

using namespace std;

void signal_handler(int sig_num)
{
    // Prevent user to exit with Ctrl+C
    signal(SIGINT, signal_handler);
    cout << "\n Oops! It seems you are trying to exit. Please use exit command\n";
    cout << "Press enter to continue";
    fflush(stdout);
}

int main(int argc, char *argv[])
{

    signal(SIGINT, signal_handler);
    create_tutorial_folder();

    if (argc > 2)
    {
        cerr << "unexpected arguments\n";
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        // batch mode
        std::ifstream fin(argv[1]);
        if (!fin)
        {
            cerr << "error: couldn't open the file\n";
            exit(EXIT_FAILURE);
        }

        roosh_loop(fin);
    }
    else
    {
        // interactive mode
        roosh_loop(std::cin);
    }

    return 0;
}