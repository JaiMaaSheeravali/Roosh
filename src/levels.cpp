#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

const char *passwd_list[] = {

    "XfwQrTY8drdyR8CDdpcgBSYq",
    "B7GXVdmXZGDgBXjUrP2aKhnc",
    "j7uvv2CwUFSujANxHnM2dM4L",
    "wdJPxNNK66cUGra6JJjbCLRe",
    "8X8jevLsYwWpZkhvDHXUWu5Q",
    "713ef6fc74479b3881d7fdb9b696a170173f5b6c"};

string curr_pass;
int curr_level = 1;
bool unlocked[6];

void print_instructions(string p)
{
    string line;
    ifstream myfile(p);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            cout << line << '\n';
        }
        myfile.close();
    }
    else
        cout << "Unable to open file.\nPlease try again.";
}

void print_no_access_to_level()
{
    cout << "Oops! you need to solve all previous levels to unlock this level. Currently you can attempt level " << curr_level << " .\n";
}

bool check_password(int level)
{
    cout << "enter password\n";
    cin >> curr_pass;
    if (curr_pass == passwd_list[level - 1])
    {
        cout << "Success!!\n";
        unlocked[level] = 1;
        if (level == 6)
            cout << "All levels completed!\n";
        else
            curr_level++;

        return 1;
    }
    else
    {
        cout << "wrong password!!\n";
        return 0;
    }
}