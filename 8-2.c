
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


int fibonacci_recursive(int n) {
    
    printf("F(%d)", n);
    if (n <= 0) {
        return 0;
    }
    else if (n == 1) {
        return 1;
    }
    else {
                return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
    }
}

int main() {
    int n;

    printf("계산할 항(N) 입력: ");

    // 
    if (scanf("%d", &n) != 1) {
        printf("정수 입력 : .\n");
        return 1;
    }

    if (n < 0) {
        printf("0 이상의 정수를 입력해야 합니다.\n");
    }
    else {
        printf("결과: %d\n", fibonacci_recursive(n));
    }

    return 0;
}
