


/*
정수 n을 입력받아서 피보나치 수열을 구한다.
순환전 구조 , 재귀적 방법 
n을 1~...이 될 때까지 진행하면서 각각의 방법에 대한 수행시간을 프로파일링
- 결과를 그래프 + 표 등을 포함해서 markdown.md 파일로 깃허브*/

#include <stdio.h>

/**
 * @brief 반복적(Iterative) 방법으로 피보나치 수열의 N번째 항을 계산합니다.
 * @param n 구하고자 하는 항의 번호
 * @return n번째 피보나치 수 (큰 값을 담기 위해 long long 사용)
 */


 
long long fibonacci_iterative(int n) {
    
    
    if (n <= 0) return 0;
    if (n == 1 || n == 2) return 1;
    

    long long first = 1;   // F(1)
    long long second = 1;  // F(2)
    long long next = 0;

      for (int i = 3; i <= n; i++) {
        next = first + second;
        first = second;  
        second = next;  
    }   

    return next;
}

int adadmain() {
    int n;

    printf("피보나치 수열 N번째 항 입력 : ");

    if (scanf_s("%d", &n) != 1) {
        printf("숫자를 입력하세요.\n");
        
        return 1;
    }

    if (n < 0) {
        printf("0 이상의 정수를 입력하세요.\n");
    }
    else {
        printf("F(%d)의 결과: %lld\n", n, fibonacci_iterative(n));
    }

    return 0;
}
