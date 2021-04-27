#include <unistd.h>
#include <limits.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>

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
       printf("\033[;32m%s\033[0m₹ ", cwd);
    } else {
       printf("\033[;32m.\033[0m₹ ");
    }
}