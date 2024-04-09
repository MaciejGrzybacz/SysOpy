//
// Created by maciejgrzybacz on 08.04.24.
//

#ifdef USE_DYNAMIC_LIBRARY
#include<dlfcn.h>
#endif

#include <stdio.h>

#ifndef USE_DYNAMIC_LIBRARY
double calc(double a, double b, int n);
#endif

int main(int argc, char** argv) {
#ifdef USE_DYNAMIC_LIBRARY
    char path[]="lib/calc_lib.so";
    void* handle = dlopen(path,RTLD_LAZY);
    if(!handle) {
        printf("Cannnot load a libary placed in %s", path);
        return 1;
    }
    double (*calc)(double a, double b, int n);
    calc=dlsym(handle,"calc");
    if(calc==NULL) {
        printf("Cannot load a function calc");
        return 1;
    }
#endif

    printf(" %f", calc(0.0, 3.0, 3000000));

#ifdef USE_DYNAMIC_LIBRARY
    dlclose(handle);
#endif
    return 0;
}