#include "print_primes.h"
#include <stdio.h>

#define MAX_PRIMES 10000

void print_primes_up_to(int N) {
    if (N < 2) return;

    int primes[MAX_PRIMES];
    int count = 0;

    for (int n = 2; n <= N; n++) {
        int is_prime = 1;

        for (int i = 0; i < count; i++) {
            int p = primes[i];

            if (p * p > n) break;
            if (n % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            if (count >= MAX_PRIMES) {
                fprintf(stderr, "소수 개수가 MAX_PRIMES(% d)를 초과했습니다.\n", MAX_PRIMES);
                return;
            }
            primes[count++] = n;
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%d\n", primes[i]);
    }
}
