#include <unistd.h>
#include <limits.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>

void print_input_format(){

    char hostname[50];

    gethostname(hostname, sizeof(hostname));
    struct passwd *p = getpwuid(getuid());

    printf("\033[;34m%s@", p->pw_name);
    printf("%s\033[0m:", hostname);

    char cwd[100];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("\033[;32m%s\033[0m₹ ", cwd);
    } else {
       printf("\033[;32m.\033[0m₹ ");
    }
}