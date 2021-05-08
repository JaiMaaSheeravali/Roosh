#include <iostream>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <vector>
#include <dirent.h>
#include <csignal>
#include <iomanip>
#include <sys/types.h>
#include "../include/launch.hpp"
#include "../include/input.hpp"
#include "../include/parse.hpp"
#include "../include/builtin.hpp"
#include "../include/complete.hpp"
#define MAX_BUFFER_SIZE 1024

#define moveforward() cout << "\033[C"
#define movebackward() cout << "\033[D"

using namespace std;

// struct which contains buffer and current index
typedef struct Buffer
{
    int position;
    // char buffer[MAX_BUFFER_SIZE];
    string buffer;
} InputBuffer;

void clear_console(struct Buffer keyboard)
{
    /* clear the output in console by placing
       cursor at appropiate position */

    int remaining = keyboard.buffer.length() - keyboard.position;
    while (remaining > 0)
    {
        moveforward();
        remaining--;
    }
    size_t size = keyboard.buffer.length();
    while (size--)
    {
        cout << "\b \b";
    }
    return;
}

void recover_console(struct Buffer keyboard)
{
    /* place the cursor at
     appropiate position after output*/

    cout << keyboard.buffer;
    int remaining = keyboard.buffer.length() - keyboard.position;
    while (remaining > 0)
    {
        movebackward();
        remaining--;
    }
    return;
}

void roosh_loop(std::istream &in)
{
    // We need to turn off the canoical mode
    // of the terminal to track every charachter
    // being entered

    static struct termios old, current;
    tcgetattr(0, &old); // store the current terminal setting
    current = old;
    current.c_lflag &= ~ICANON; // allow auto submit as soon as charachter is entered
    current.c_lflag &= ~ECHO;   // turn of the automatic display of input
    current.c_cc[VMIN] = 1;
    current.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &current); // set the new settings

    vector<string> history;

    int historyCount = 0; // The total number of commands that the user entered
    char keyword;
    int historyIndex = -1;
    InputBuffer keyboard = {.position = 0, .buffer = ""}; // contain the input
    string temp;
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
                    temp = keyboard.buffer;
                }

                historyIndex++;
                if (historyIndex >= historyCount)
                {
                    historyIndex = historyCount - 1;
                }
                else if (historyIndex > (int)history.size() - 1)
                {
                    historyIndex = history.size() - 1;
                }

                break;

            case 'B': // Down-arrow
                historyIndex--;

                if (historyIndex < -1)
                {
                    historyIndex = -1;
                }
                break;

            case 'C': // Right arrow
                if (keyboard.position >= (int)keyboard.buffer.length())
                {
                    continue;
                }
                moveforward();
                keyboard.position++;
                continue;
                break;

            case 'D': // Left arrow
                if (keyboard.position <= 0)
                {
                    continue;
                }
                keyboard.position--;
                movebackward();
                continue;
                break;
            default:
                continue;
            }

            // Clear output in console
            clear_console(keyboard);

            // Replaces the string in the buffer with what the user typed in
            if (historyIndex == -1)
            {
                keyboard.buffer = temp;
            }

            // Replaces the string in the buffer with a previous command using the arrow key
            else
            {
                keyboard.buffer = history[historyIndex];
            }

            keyboard.position = keyboard.buffer.length();

            cout << keyboard.buffer;
        }
        else if (keyword == 0x7f)
        {
            // If Backspace charachter is used
            if (!keyboard.position)
            {
                continue;
            }
            clear_console(keyboard);
            // remove the keyword from appropiate position
            keyboard.buffer.erase(keyboard.position - 1, 1);
            keyboard.position--;
            recover_console(keyboard);
        }
        else if (keyword == '~')
        {
            // If Delete key is used
            if (keyboard.position == (int)keyboard.buffer.length())
            {
                continue;
            }
            clear_console(keyboard);
            // remove the keyword from appropiate position
            keyboard.buffer.erase(keyboard.position, 1);
            recover_console(keyboard);
        }
        else if (keyword == '\n')
        {
            cout << "\n";
            string line = keyboard.buffer;
            // empty command i.e 'enter key'
            if (line.empty())
            {
                print_input_format();
                continue;
            }

            // pushing 'line' into the history
            push_command(line);

            // execute all the commands specified in the line
            bool status = roosh_launch(line);

            history.insert(history.begin(), keyboard.buffer);

            historyCount++;

            // status equals false when
            // exit command is run by the user
            if (status == false)
            {
                tcsetattr(0, TCSANOW, &old); // restore original terminal settings
                exit(EXIT_SUCCESS);
            }

            keyboard.buffer = "";
            keyboard.position = 0;
            historyIndex = -1;

            print_input_format();
            fflush(stdout);
        }
        else if (keyword == '\t')
        {
            // If tab key is pressed
            vector<string> files = complete(keyboard.buffer);
            if (files.size() != 0)
            {
                if (files.size() == 1)
                {
                    string temp1;
                    int index = keyboard.buffer.find_last_of("/");
                    if (index == -1)
                    {
                        index = keyboard.buffer.find_last_of(" ");
                        if(index!=-1){
                            temp1 = keyboard.buffer.substr(0, index+1);
                        }
                        else{
                            temp1 = "";
                        }
                    }
                    else{
                        temp1 = keyboard.buffer.substr(0, index+1);
                    }
                    clear_console(keyboard);
                    keyboard.buffer = temp1 + files[0];
                    keyboard.position = keyboard.buffer.length();
                    cout << keyboard.buffer;
                }
                else
                {
                    cout << "\n";
                    int count = 0;
                    for (auto i = files.begin(); i != files.end(); i++)
                    {
                        if (count == 4)
                        {
                            cout << "\n";
                            count = 0;
                        }
                        cout << left << setw(40) << setfill(' ') << *i;
                        count++;
                    }
                    cout << "\n";
                    print_input_format();
                    cout << keyboard.buffer;
                }
            }
        }
        else
        {
            // if any other charachter is used
            // store it in buffer

            clear_console(keyboard);
            // insert keyword at appropiate position
            keyboard.buffer.insert(keyboard.position, string(1, keyword));
            keyboard.position++; // update position
            recover_console(keyboard);
        }
    }
    tcsetattr(0, TCSANOW, &old);
}