#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include "my_math.h"

// n번째 피보나치 수를 구하는 함수 (재귀 대신 반복문으로 구현하여 속도 확보)
long long get_fibonacci(int n) {
    if (n <= 1) return n;
    long long prev = 0, curr = 1, next;
    for (int i = 2; i <= n; i++) {
        next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

int main() {
    int start_n = 5;
    int end_n = 90; // long long 범위 내에서 테스트 가능한 최대치 근접

    printf("=== 유클리드 호제법 성능 프로파일링 (Fibonacci) ===\n");
    printf("%-5s | %-20s | %-20s | %-10s | %-10s\n", "n", "F(n)", "F(n-1)", "GCD", "Time(s)");
    printf("--------------------------------------------------------------------------\n");

    for (int n = start_n; n <= end_n; n++) {
        long long fn = get_fibonacci(n);
        long long fn_minus_1 = get_fibonacci(n - 1);

        // 1. CPU 시간 측정 시작
        clock_t start_time = clock();

        // 2. GCD 계산 실행 (알고리즘 부하 테스트)
        int result = 0;
        for (int i = 0; i < 1000000; i++) {
            result = get_gcd((int)fn, (int)fn_minus_1);
        }

        // 3. 시간 측정 종료
        clock_t end_time = clock();
        double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        // 결과 출력
        printf("%-5d | %-20lld | %-20lld | %-10d | %.10f\n",
            n, fn, fn_minus_1, result, duration);
    }

    return 0;
}
