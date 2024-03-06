//
// Created by maciejgrzybacz on 02.03.24.
//

#include <stdio.h>

extern char** environ;

int main() {
    char** var;
    for(var=environ; *var!=NULL; var++) {
        printf("%s\n", *var);
    }
    return 0;
}
