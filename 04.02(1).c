#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n;

    
    printf("배열의 크기(n)를 입력하세요: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("올바른 양의 정수를 입력하세요.\n");
        return 1;
    }

    
    int* arr = (int*)malloc(sizeof(int) * n);

    if (arr == NULL) {
        printf("메모리 할당 실패!\n");
        return 1;
    }

    int sum = 0;
    int max, min;

    srand((unsigned int)time(NULL));

        for (int i = 0; i < n; i++) {
        
        *(arr + i) = rand() % (n + 1);
    }

    // 4. 정방향 출력
    printf("\n--- 정방향 출력 (0 -> %d) ---\n", n - 1);
    for (int i = 0; i < n; i++) {
        printf("%2d ", *(arr + i));
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");

    // 5. 역방향 출력
    printf("--- 역방향 출력 (%d -> 0) ---\n", n - 1);
    for (int i = n - 1; i >= 0; i--) {
        printf("%2d ", *(arr + i));
        if ((i) % 10 == 0) printf("\n");
    }

    
    max = min = *(arr + 0);

    for (int i = 0; i < n; i++) {
        int current_val = *(arr + i);

        sum += current_val;
        if (current_val > max) max = current_val;
        if (current_val < min) min = current_val;
    }

    printf("\n--- 결과 분석 ---\n");
    printf("총합   : %d\n", sum);
    printf("최대값 : %d\n", max);
    printf("최솟값 : %d\n", min);

    
    free(arr);
    printf("\n메모리 해제가 완료되었습니다.\n");

    return 0;
}
