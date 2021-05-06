#include <iostream>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
using namespace std;

vector<string>  complete(string keyboard){
    vector<string> files;
    DIR *dr;
    struct dirent *en;
    size_t index = keyboard.find_last_of(" ");
    string path = ".";
    if(index != string::npos && index!=keyboard.length()-1){
        keyboard = keyboard.substr(index+1);
        index = keyboard.find_last_of("/");
        if(index!=string::npos){
            path=keyboard.substr(0,index);
            keyboard = keyboard.substr(index+1);
        }
    }
    if(keyboard==""){
        return files;
    }
    dr = opendir(path.c_str());
    if(dr){
        while((en = readdir(dr)) != NULL){
            string dir(en->d_name);
            if(dir=="." || dir==".."){
                continue;
            }
            if(dir.compare(0,keyboard.length(), keyboard )==0){
                files.push_back(en->d_name);
            }
        }
        closedir(dr);
    }
    return files;
}
