//
// Created by maciejgrzybacz on 06.03.24.
//

#include <stdio.h>
#include "collatz_library/collatz.h"

int main() {
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