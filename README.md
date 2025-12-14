# 2025-2 System Programming (CSE3209-001)

본 문서는 인하대학교 **시스템프로그래밍(CSE3209)** 교과목의 Static Linking, Dynamic Linking, Runtime Linking에 관한 구현 내용을 포함합니다.  

정수(N) 입력 시, 해당 정수 이하의 모든 소수를 출력해주는 함수를 디자인하고 링킹하는 과정을 설명합니다.

---

### 💻 개발 환경

- **OS** : Ubuntu 20.04 LTS  
- **Compiler** : GCC 9.4.0  

---


### 📁 파일 구조

```bash
├── static
│   ├── main.c
│   ├── print_primes.c
│   ├── print_primes.h
│   ├── print_primes.o
│   ├── libprime.a
│   └── main_static
│
├── dynamic
│   ├── main.c
│   ├── print_primes.c
│   ├── print_primes.h
│   ├── print_primes.o
│   ├── libprime.so
│   └── main_dynamic
│
├── runtime
│   ├── main_runtime.c
│   ├── print_primes.c
│   ├── print_primes.h
│   ├── print_primes.o
│   ├── libprime.so
│   └── main_runtime
│
└── README.md
```

### 코드 설명
`print_primes.h`

다음의 .h 파일은 print_primes_up_to() 함수 프로토 타입을 선언합니다.
```c
#ifndef PRINT_PRIMES_H
#define PRINT_PRIMES_H

void print_primes_up_to(int N);

#endif
```
`print_primes.c`

다음의 .c 파일은 헤더 파일을 포함하고, 함수 본문을 구현합니다.

입력된 정수 N 이하(2~N) 의 모든 소수를, 이미 찾은 소수들만으로 나눠보는 방식으로 판별하고 출력하는 함수입니다.
```c
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
```
`main.c`

라이브러리를 사용하는 main 코드 입니다.

print_primes_up_to() 함수를 사용하여 N 이하의 모든 소수를 출력합니다.
```c
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
```
`main_runtime.c`

Runtime Linking에 사용하는 main 코드 입니다.

dlopen과 dlsym을 사용해 런타임에 동적으로 라이브러리를 로드하고 print_primes_up_to 함수를 호출합니다.
```c
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
```
## Static Linking
해당 실행 과정은 static 폴더에서 이루어집니다.
```bash
$ gcc -Og -c print_primes.c -o print_primes.o
$ ar -rcs libprime.a print_primes.o
$ gcc -static main.c -L. -lprime -o main_static
```
<img width="2024" height="674" alt="static_linking" src="https://github.com/user-attachments/assets/54d1d0be-83d6-4973-9338-c5e959727a7d" />

## Dynamic Linking
해당 실행 과정은 dynamic 폴더에서 이루어집니다.
```bash
$ gcc -Og -c -fPIC print_primes.c -o print_primes.o
$ gcc -shared -o libprime.so print_primes.o
$ gcc main.c -L. -lprime -o main_dynamic
$ export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
```
<img width="2130" height="716" alt="dynamic_linking" src="https://github.com/user-attachments/assets/3fd808d7-f701-4f25-a311-b7f8021915e4" />

## Runtime Linking
해당 실행 과정은 runtime 폴더에서 이루어집니다.
```bash
$ gcc -fPIC -C print_primes.c -o print_primes.o
$ gcc -shared -o libprime.so print_primes.o
$ gcc -rdynamic -o main_runtime main_runtime.c -ldl
$ export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
```
<img width="2140" height="732" alt="runtime_linking" src="https://github.com/user-attachments/assets/de502a16-9b40-477f-a8dd-d7908ce08b4a" />


