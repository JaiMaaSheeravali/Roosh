#include <iostream>
#include <string>
#include <utility>
#include <fstream>

#include "../include/parse.hpp"
#include "../include/launch.hpp"
using namespace std;

void roosh_loop(std::istream &in)
{
    string line;

    // keep taking the input till EOF or exit
    while (getline(in, line))
    {
        // empty command i.e 'enter key'
        if (line.empty())
        {
            continue;
        }

        auto [args, n] = roosh_parse(line);
        bool status = roosh_launch(args, n);

        // status equals false when
        // exit command is run by the user
        if (status == false)
        {
            exit(EXIT_SUCCESS);
        }

        // deallocating memory
        for (int i = 0; i < n; i++)
        {
            delete[] args[i];
        }
        delete[] args;
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