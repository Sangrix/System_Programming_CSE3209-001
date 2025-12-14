#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void* handle;
    void (*print_primes_up_to)(int);
    char* error;
    
    int N;

    printf("정수를 입력해주세요: ");
    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "입력 오류\n");
        exit(1);
    }

    handle = dlopen("./libprime.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    print_primes_up_to = dlsym(handle, "print_primes_up_to");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    print_primes_up_to(N);

    if (dlclose(handle) < 0) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    return 0;
}

