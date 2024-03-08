//
// Created by maciejgrzybacz on 06.03.24.
//
#include "collatz.h"

int collatz_conjecture(int input) {
    if (input % 2 == 0)
        return input / 2;
    else
        return 3 * input + 1;
}

int test_collatz_convergence(int input, int max_iter) {
    if (input == 0)
        return -1;

    int i = 0;

    while (input> 1 && i < max_iter) {
        input = collatz_conjecture(input);
        i++;
    }

    if (i == max_iter) {
        return -1;
    }
    return i;
}