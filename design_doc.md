## Overview
- [Overview](#overview)
- [Introduction](#introduction)
- [High-Level Design](#high-level-design)
  * [Data Flow Diagrams](#data-flow-diagrams)
  * [Activity Diagrams](#activity-diagrams)
    + [Roosh Overview](#roosh-overview)
    + [Input Handling](#input-handling)
    + [Roosh Calc](#roosh-calc)
    + [Pipes and Redirections](#pipes-and-redirections)
    + [Roosh Google](#roosh-google)
    + [Roosh Tutorial](#roosh-tutorial)
    + [Roosh Tutorial Practice](#roosh-tutorial-practice)
  * [Sequence Diagram](#sequence-diagram)
    + [Roosh Google](#roosh-google-1)
- [Low-Level Design](#low-level-design)
    + [Roosh Input Prompt](#roosh-input-prompt)
    + [Roosh Loop](#roosh-loop)
    + [Roosh Parser](#roosh-parser)
    + [Roosh Tokenizer](#roosh-tokenizer)
    + [Roosh Launcher](#roosh-launcher)
    + [Roosh Pipe](#roosh-pipe)
    + [Roosh Google Command](#roosh-google-command)
    + [Roosh Calc Command](#roosh-calc-command)
    + [Roosh Tutorial Command](#roosh-tutorial-command)

---

## Introduction

We have built a shell in C++ called [Roosh](https://github.com/JaiMaaSheeravali/Roosh). Beginners face a lot of problems learning Linux and its commands. Roosh will be helpful to beginners. We are adding tutorials in Roosh, which will teach commands and give practice exercises, and those exercises will be graded. So these tutorials can help beginners in learning the Linux commands. All the basic functionalities of a shell are wrapped together in a single place, so users need not go to different platforms to learn different functionalities. Following are the features provided in the Roosh.

- Google using shell without a web browser
- Tab auto-complete and history access through arrow keys
- Color coding
- Aliases and Shell arithmetic
- Interactive shell along with the batch mode
- Pipes and Redirections
- Display Git Branch Status
- Play and learn through Tutorials
---

## High-Level Design

### Data Flow Diagrams

![](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/dfds/DFD-Shell-Page-1.svg)

![](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/dfds/DFD-Shell-Page-2.svg)

![](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/dfds/DFD-Shell-Page-3.svg)

![](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/dfds/DFD-Shell-Page-5.svg)
---

### Activity Diagrams

#### Roosh Overview

![Overview](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/Roosh.svg)

---

#### Input Handling

![Signal](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/Roosh_Signal.svg)

---

#### Roosh Calc

![Calculator](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/Roosh_calc.svg)

---

#### Pipes and Redirections

![Pipes](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/Roosh_pipe.svg)

----

#### Roosh Google

![Google Activity Diagram](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/Roosh_google.svg)

---

#### Roosh Tutorial

![Tutorial Diagram](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/Roosh_tutorial.svg)
---

#### Roosh Tutorial Practice

![Tutorial Practice](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/Roosh_tut_practice.svg)

---

### Sequence Diagram

#### Roosh Google

![Google Sequence Diagram](https://raw.githubusercontent.com/JaiMaaSheeravali/Roosh/master/svgs/Google%20HTTps.svg)

## Low-Level Design

#### Roosh Input Prompt

Like Bash shell, Roosh shell shows several details like username, hostname, current working directory in the input prompt. As a distinct feature, Roosh Input Prompt shows the rupee-symbol "₹" in place of "$" in the bash shell. Color coding of these details is also followed for a better aesthetic feel and ease of distinction. We also tried a new feature especially useful for developers, Roosh Input Prompt also shows the name of the current branch if the user is working in a git directory. It is a handy feature for project developers using git as VCS.

```c++
void print_input_format()
{

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

    cout << ":" << GREEN;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        cout << cwd;
    else
        cout << ".";
    cout << RESET;

    // Get git branch if current directory is a git directory
    print_git_branch();
    cout << "₹ " << flush;
}
```

Git branch is obtained by running the `git branch --show-current` command using `popen` system call and redirecting errors to `/dev/null` file if the current directory is not a git directory. This method is used because git doesn't have any system call implemented in C/C++ directly for this purpose.
```c++
void print_git_branch()
{
    FILE *fp;
    char branch_name[400];

    // Get current branch and save its name in a file.
    fp = popen("git branch --show-current 2>/dev/null", "r");

    if (fp == NULL)
    {
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
```

---


#### Roosh Loop

Roosh Loop repeatedly displays the input prompt and accept the command that the user types to execute. For that, we first need to turn off the canonical mode of the terminal. This will help in parsing each character pressed by the user and take action accordingly. 
Following code snippet describes the changes to be done in terminal settings.

```c++
static struct termios old, current;
    tcgetattr(0, &old); /* store the current terminal setting */
    current = old;
    current.c_lflag &= ~ICANON; /* allow auto submit as soon as charachter is entered */
    current.c_lflag &= ~ECHO;   /* turn of the automatic display of input */
    current.c_cc[VMIN] = 1;
    current.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &current); /* set the new settings */
```

We will store the input using a `struct Buffer` which will contain an `int position` that will store the next position to be filled, and a `string buffer` to store the actual command.

After that, we need to identify each character being pressed by the user and act accordingly.

```c++
int historyCount = 0; // The total number of commands that the user entered
int historyIndex = -1;
InputBuffer keyboard;
while (1)
    {
        keyword = getchar();
        .......
    }
```
If the pressed key is a left-right arrow key, then we need to move the cursor accordingly. If it is an up-down arrow key, then we should access the history.
```c++
        if (keyword == 27)
        {
            // If the key is Esc
            keyword = getchar();

            switch (getchar())
            {
            case 'A': // Up-arrow
                
                store_current_command();
                historyIndex++;
                set_history_index();
                
            case 'B': // Down-arrow
                historyIndex--;
                set_history_index();

            case 'C': // Right arrow
                moveforward(); // move cursor forward
                keyboard.position++;
                continue;

            case 'D': // Left arrow
                keyboard.position--;
                movebackward();
                continue;
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
```
If the back arrow key is pressed, then we need to delete the character and change the buffer accordingly.
```c++
        else if (keyword == 0x7f) // Backspace
        {
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
```
If Enter is pressed, then that command needs to be executed with the help of `roosh_launch`.
```  c++
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
            tcsetattr(0, TCSANOW, &old); // restore original terminal settings
            bool status = roosh_launch(line);
            tcsetattr(0, TCSANOW, &current); // set the new settings
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
        }
```
If the tab key is pressed, then we need to call the `complete` method, which would return the list of files with given initials. If only one option is possible, it gets completed; otherwise, the list of files with the given initials would be displayed.

```c++
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
                
```
If the pressed key is none from the characters checked above, it is simply added to the buffer, and the position is updated. In this way, the input of the user is handled. And when the user exits the shell, the terminal should be brought back to its original settings, therefore, we should add 
 `tcsetattr(0, TCSANOW, &old);` in the end.

---

#### Roosh Parser

Roosh parser is used when we have to run a single command, and the command is in the form of `string`. So we have to convert the command from `string cmd`  to `char** cmd`.

Following code describes how `roosh_parse`  works

```
roosh_parse(string command)
    1. Create a list_args to store each argument of the command
    2. Create an iterator for iterating over the command
    3. Use the above iterator and add each argument to the list_args
    4. Convert the list_args to array of cstrings (char **)
```

---

#### Roosh Tokenizer

When the `launcher` function described below has to run, it uses `roosh_tokenizer` to divide the commands into several pieces. For example, if a command has a structure like 

```
command_1 | command_2 | command_3 | . . . . . .     |  command_n
```

then the tokenizer will split into a list of commands with `|` as delimiter in the following manner.
```
index           list
[0]    ---->    command_1
[0]    ---->    command_2
[0]    ---->    command_3
 .                 .
 .                 .
 .                 .
[n-1]  ---->    command_3
```

Following code describes how `roosh_tokenizer` works

```c++
// accepts a string line as input and
// tokenizes it according to delimeter
roosh_tokenizer(line, delimiter)
{
    vector<string> tokens;	 // to store tokens
    string token;            // a single token
    
    istringstream iss(line); // used to split string around delimiter

    // Tokenizing w.r.t. specified delimiter
    while (getline(iss, token, delimiter))
    {
        token = trim(token);  // cleanup whitespaces from token
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}
```

---

#### Roosh Launcher

Roosh launcher is used when a user types a line and clicks enter key. The launcher has to do all the preparations before sending the input for execution.

Following code describes how `roosh_launch`  works

```
roosh_launch(string line)
    1. Call roosh_tokenizer to generate a list of commands
    2. Set the input source for the first command. (stdin or a file)
    3. Send the list_cmds to the pipe_exec for execution
```

---

#### Roosh Pipe

Roosh pipes are used for actually executing all the commands received from `roosh_launch`. The process flow of Roosh launch and pipe is explained in a detail in the *activity diagram*. 

Following code describes how `pipe_exec`  works

```c++
bool pipe_exec(const vector<string> &cmds, size_t pos, int in_fd)
{
    string cmd = cmds[pos];

    if (pos == cmds.size() - 1) /* last command */
    {
        return pipe_exec_last(cmd, in_fd);
    }
    else
    {
        int fd[2]; /* output pipe */
        if (pipe(fd) == -1)
        {
            report_error_and_exit("pipe");
        }

        // parent checks for builtin
        int idx = check_builtin(cmd);

        // if it is a builtin command, parent runs it
        // and moves on to the next command without forking
        if (idx != -1)
        {
            pipe_redirect(in_fd, STDIN_FILENO);  // read from read_end of old pipe/stdin
            pipe_redirect(fd[1], STDOUT_FILENO); // write to the write_end of new pipe

            run_builtin(cmd, idx);                  /* run builtin-command */
            return pipe_exec(cmds, pos + 1, fd[0]); /* execute the rest */
        }

        /* fork if the command is not a builtin */
        pid_t pid = fork();
		
        /* fork failed */
        if (pid < 0)
        {
            report_error_and_exit("fork");
        }
        
        /* child process */
        else if (pid == 0)
        {
            child = 1;
            Close(fd[0]);                        /* unused */
            
            pipe_redirect(in_fd, STDIN_FILENO);  /* read from in_fd */
            pipe_redirect(fd[1], STDOUT_FILENO); /* write to fd[1] */

            auto [args, num_args] = roosh_parse(cmd);
            execvp(args[0], args);
            report_error_and_exit("execvp");
        }

        /* parent process */
        
        Close(fd[1]); /* unused */
        
        /* closes the old pipe */
        if (in_fd != STDIN_FILENO)
        {
            Close(in_fd);
        }

        /* waits for the child process to complete */
        wait(NULL);
        
        /* execute the rest of the commands*/
        return pipe_exec(cmds, pos + 1, fd[0]); 
    }
}
```

In the code above, first, we check whether the command to be executed is the last command. Because the last command works slightly differently, it does not create a new pipe and does not make a recursive call. It just ends the whole cycle of running new commands.

---


#### Roosh Google Command

Roosh Google command is used to get meanings of a word via a request sent to [Free Dictionary API](https://dictionaryapi.dev/). The command can return meaning to a word in more than six languages that can be specified using language code. User can access the help page of the google command via `google --help` command that lists the language codes too, along with the format of command, i.e. `google <word> <language>` or to use default language `google <word>` is enough. 

Following is the code for sending a request and parsing the JSON response received from the API. [CPR](https://github.com/whoshuu/cpr) is used to make HTTPS requests and [nlohmann](https://github.com/nlohmann/json) to parse JSON from the string. 

```c++
int google_a_description(string str, string lang)
{
    try
    {
        using json = nlohmann::json;
        str = "https://api.dictionaryapi.dev/api/v2/entries/" + lang + "/" + str;

        cpr::Response r = cpr::Get(
            cpr::Url{str}, cpr::Timeout{5000},
            cpr::Header{{"accept", "application/json"},
                        {"user-agent",
                         "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.93 Safari/537.36"},
                        {"upgrade-insecure-requests", "1"}},
            cpr::VerifySsl{false});

        auto j = json::parse(r.text);
        for (auto ar : j[0].at("meanings"))
        {
            cout << ar.at("partOfSpeech") << "  " << ar.at("definitions")[0].at("definition") << endl;
        }
    }
    catch (...)
    {
        cout << "Sorry this word cannot be found at this moment\n";
    }
    return 0;
}
```

To Capture the parsed command format and handle the help page following code block is used:
```c++
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
            Print Google Help Page
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
```

The code checks the number of arguments passed by the Roosh parser and, based on it, analyze the condition and calls the function to fetch data accordingly.

---

#### Roosh Calc Command

Roosh Calc command is used to evaluate simple arithmetic expressions and displays the output. For this command we will use [tinyexpr](https://github.com/codeplea/tinyexpr) library written in C. 
```c++
int roosh_calc(char **args, int num_args)
{
    // 2 arguments required
    // first is the calc command
    // second is the expresssion given
    if (num_args > 2)
    {
        invalid_arg_count_error(num_args, 1);
        return 1;
    }
    if (num_args == 1 || !strcmp(args[1], "--help"))
    {    
        Print Calc Help Page
        return 1;
    }

    // tinyexpr evaluates the expresssion
    double answer = te_interp(args[1], 0);
    cout << answer << endl;

    return 1;
}
```

---

#### Roosh Tutorial Command

Our Tutorial only provides a basic usage overview of most commonly used commands in linux and certain test levels to test your understanding of these concepts.
The tutorial is implemented in two parts: 
a) ```tutorial``` command displays the tutorial content.
b) ```tutorial level [level-number]``` to access the levels and ```tutorial level [level-number]``` password to submit the password for grading.

To implement these from the parsed command format, following function block is used: 
```c++
int roosh_exec_tutorial(char **args, int num_args)
{
    // number of arguments can be =>
    // 1 i.e. tutorial
    // 3 i.e. tutorial level <1-6>
    // 4 i.e. tutorial level <1-6> password

    if (num_args == 1)
    {
        return roosh_tutorial();
    }

    // case of 3 or 4 arguments
    else if ((num_args == 3 || num_args == 4) && (strcmp(args[1], "level") == 0))
    {
        if (num_args == 4)
        { 	
            //error if fourth arg is not password
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

        //level must be b/w 1-6
        if (level > 0 && level <= 6 && (strlen(args[2]) == level / 10 + 1))
        {

            if (num_args == 3)
            {
                if (level == 1 || unlocked[level - 1])
                {
                    //display corresponding instructins
                    string path = (home_dir + "/tutorial/lvl_instrns/level" + args[2] + ".txt");
                    print_instructions(path);

                    //navigate to corresponding directory
                    path = (home_dir + "/tutorial/tutorial_levels/level" + args[2]);
                    chdir(path.c_str());
                    if (level == 4)
                    {

                        roosh_launch("chmod 000 " + home_dir + "/tutorial/tutorial_levels/level4/open_me");
                    }
                }
                else
                {
                    print_no_access_to_level();
                }
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
            //display error if level is not an integer bw 1 nd 6
            cerr << "Please enter a level between 1 to 6.\n";
            return 1;
        }
    }

    else
    {
        cerr << "no such command exists\n";
        return 1;
    }
}
```
