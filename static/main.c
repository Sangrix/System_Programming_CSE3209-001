#include <stdio.h>
#include "print_primes.h"

int main(void) {

    int N;

    printf("정수를 입력해주세요: ");

    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "입력 오류\n");
        return 1;
    }

    print_primes_up_to(N);
    return 0;
}
