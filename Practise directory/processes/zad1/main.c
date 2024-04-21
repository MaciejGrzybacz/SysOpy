#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dlfcn.h>

int main (int l_param, char * wparam[]){
    int i;
    int tab[20]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};

    void *handle = dlopen("bibl.so", RTLD_LAZY);

    int (*f1) (int, int) = dlsym(handle, "sumuj");
    double (*f2) (int, int) = dlsym(handle, "dziel");
    for (i=0; i<3; i++) printf("Wynik: %d, %lf\n", f1(tab[i], 20-i), f2(tab[i], tab[i+1]));
    dlclose(handle);
    return 0;
}