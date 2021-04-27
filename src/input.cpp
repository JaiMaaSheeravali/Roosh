#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>

void print_git_branch();

void print_input_format(){

    char hostname[50];
    struct passwd *p = getpwuid(getuid());

    // Get the host name and user name of current user
    if(gethostname(hostname, sizeof(hostname)) == 0 && p){
        printf("\033[;34m%s@", p->pw_name);
        printf("%s\033[0m:", hostname);
    }

    // Print the current working directory of user
    char cwd[100];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("\033[;32m%s\033[0m", cwd);
    } else {
       printf("\033[;32m.\033[0m");
    }

    // Get git branch if current directory is a git directory
    print_git_branch();
    printf("₹ ");
}

void print_git_branch(){
    FILE *fp;
    char path[400];
    
    // Get current branch and save its name in a file.
    fp = popen("git branch --show-current 2>/dev/null", "r");

    if (fp == NULL)
        return;
    
    // Read branch name and print current branch name
    fgets(path, 200, fp);
    path[strcspn(path, "\n")] = 0;
    
    if(path && strcmp(path,"") == 0)
        return;
    
    // print current branch name in red enclosed in round braces
    printf(" \033[31m(%s)\033[0m",path);
    
    // Should check status if any error occurs
    pclose(fp);
}