//
// Created by maciejgrzybacz on 08.04.24.
//

#include "calc_lib.h"

double f(double x) {
    return x*x;
}

double calc(double a, double b, int n) {
    double result=0.0;
    double h=(b-a)/n;
    for(int i=0; i<n; i++) {
        result+=f(a+i*h);
    }
    return result*h;
}
