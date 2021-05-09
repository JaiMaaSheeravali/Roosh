#include "../include/launch.hpp"

#include <cstring>
#include <string>
#include <vector>

#include "../include/parse.hpp"
#include "../include/pipe.hpp"
#include "../include/redirect.hpp"
#include "../include/tutorial.hpp"

using namespace std;

bool roosh_launch(const string &line)
{
    vector<string> cmds = roosh_tokenizer(line, '|');
    // take input for first command
    // from a file if required
    int in_fd = redirect_input(cmds[0]);

    // if invalid file was mentioned just continue
    if (in_fd == -1)
    {
        return 1;
    }

    return pipe_exec(cmds, 0, in_fd);
}