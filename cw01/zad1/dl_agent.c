//
// Created by maciejgrzybacz on 06.03.24.
//

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char path[] = "collatz_library/collatz.so";
    void* handle = dlopen(path,RTLD_LAZY);
    if(!handle) {
        printf("Cannnot load a libary placed in %s", path);
        return 1;
    }

    int (*collatz_conjecture)(int input);
    int (*test_collatz_convergence)(int input, int max_iter);

    collatz_conjecture = dlsym(handle, "collatz_conjecture");
    test_collatz_convergence = dlsym(handle, "test_collatz_convergence");

    if(collatz_conjecture == NULL) {
        printf("Cannot load collatz_conjecture function from %s", path);
        return 1;
    }
    if(test_collatz_convergence == NULL) {
        printf("Cannot load test_collatz_convergence function from %s", path);
        return 1;
    }

    int max_iter = 10;
    for (int i = 0; i < 10; i++) {
        int collatz_convergence = test_collatz_convergence(i, max_iter);
        if (collatz_convergence >= 0)
            printf("Collatz conjecture converges for %d after %d operations.\n", i, collatz_convergence);
        else
            printf("Collatz conjecture for %d is unreachable after %d operations.\n", i, max_iter);
    }
    return 0;
}
