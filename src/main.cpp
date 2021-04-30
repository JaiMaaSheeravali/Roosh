#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <string.h>
#include <termios.h>
#include "../include/parse.hpp"
#include "../include/launch.hpp"
#include "../include/input.hpp"
#include "../include/builtin.hpp"
#define MAX_BUFFER_SIZE 1024
#define HISTORY_BUFFER_SIZE 10
using namespace std;

// struct which contains buffer and current index
typedef struct Buffer
{
    int position;
    char buffer[MAX_BUFFER_SIZE];
} InputBuffer;

void roosh_loop(std::istream &in)
{
    // We need to turn off the canoical mode
    // of the terminal to track every charachter
    // being entered

    static struct termios old, current;
    tcgetattr(0, &old); // store the current terminal setting
    current = old;
    current.c_lflag &= ~ICANON;      // allow auto submit as soon as charachter is entered
    current.c_lflag &= ~ECHO;        // turn of the automatic display of input
    tcsetattr(0, TCSANOW, &current); // set the new settings

    char *history[HISTORY_BUFFER_SIZE]; // Holds 10 of the most recent commands
    for (int i = 0; i < HISTORY_BUFFER_SIZE; i++)
    {
        history[i] = (char *)malloc(sizeof(char) * MAX_BUFFER_SIZE);
    }

    int historyCount = 0; // The total number of commands that the user entered
    char keyword;
    int historyIndex = -1;
    InputBuffer keyboard = {.position = 0}; // contain the input
    char *temp = (char *)malloc(sizeof(char *));
    print_input_format();

    // keep taking the input till EOF or exit
    while (1)
    {
        keyword = getchar();

        if (keyword == 27)
        {
            // If the key is Esc
            keyword = getchar();

            switch (getchar())
            {
            case 'A': // Up-arrow

                if (historyIndex == -1)
                {
                    // Store the current string temporarily
                    keyboard.buffer[keyboard.position] = '\0';
                    strcpy(temp, keyboard.buffer);
                }

                historyIndex++;
                if (historyIndex >= historyCount)
                {
                    historyIndex = historyCount - 1;
                }
                else if (historyIndex > 9)
                {
                    historyIndex = 9;
                }

                break;

            case 'B': // Down-arrow
                historyIndex--;

                if (historyIndex < -1)
                {
                    historyIndex = -1;
                }
                break;

            default:
                continue;
            }

            /* Clear output in console*/
            size_t size = strlen(keyboard.buffer);
            while (size--)
            {
                cout << "\b \b";
            }

            // Replaces the string in the buffer with what the user typed in
            if (historyIndex == -1)
            {
                strcpy(keyboard.buffer, temp);
            }

            // Replaces the string in the buffer with a previous command using the arrow key
            else
            {
                strcpy(keyboard.buffer, history[historyIndex]);
            }

            keyboard.position = (int)strlen(keyboard.buffer);

            cout << string(keyboard.buffer);
        }

        else if (keyword == 0x7f)
        {
            // If Backspace charachter is used
            if (!keyboard.position)
            {
                continue;
            }
            cout << "\b \b";
            keyboard.buffer[--keyboard.position] = '\0';
        }

        else if (keyword == '\n')
        {
            keyboard.buffer[keyboard.position] = '\0';
            cout << "\n";
            string line(keyboard.buffer);

            // empty command i.e 'enter key'
            if (line.empty())
            {
                print_input_format();
                continue;
            }

            // pushing 'line' into the history
            push_command(line);

            auto [args, n] = roosh_parse(line);
            bool status = roosh_launch(args, n);

            int size = HISTORY_BUFFER_SIZE - 1;

            while (size--)
            {
                // shift the element in the array
                strcpy(history[size + 1], history[size]);
            }

            strcpy(history[0], keyboard.buffer);

            historyCount++;

            // status equals false when
            // exit command is run by the user
            if (status == false)
            {
                free(temp);
                for (int i = 0; i < HISTORY_BUFFER_SIZE; i++)
                {
                    free(history[i]);
                }
                tcsetattr(0, TCSANOW, &old); // restore original terminal settings
                exit(EXIT_SUCCESS);
            }

            keyboard.position = 0;
            historyIndex = -1;

            // deallocating memory
            for (int i = 0; i < n; i++)
            {
                delete[] args[i];
            }
            delete[] args;

            print_input_format();
            fflush(stdout);
        }
        else
        {
            // if any other charachter is used
            // store it in buffer
            keyboard.buffer[keyboard.position++] = keyword;
            cout << keyword;
        }
    }
    tcsetattr(0, TCSANOW, &old);
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