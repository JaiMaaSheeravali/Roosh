#include <iostream>
#include <string>
#include <utility>
#include <fstream>

#include "../include/parse.hpp"
#include "../include/launch.hpp"
#include "../include/input.hpp"
using namespace std;

void roosh_loop(std::istream &in)
{
    string line;
    print_input_format();

    // keep taking the input till EOF or exit
    while (getline(in, line))
    {

        // empty command i.e 'enter key'
        if (line.empty())
        {
            print_input_format();
            continue;
        }

        auto [args, num_args] = roosh_parse(line);
        bool status = roosh_launch(args, num_args);

        // status equals false when
        // exit command is run by the user
        if (status == false)
        {
            exit(EXIT_SUCCESS);
        }

        // deallocating memory
        for (int i = 0; i < num_args; i++)
        {
            delete[] args[i];
        }
        delete[] args;

        print_input_format();
    }
}

int main(int argc, char *argv[])
{

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