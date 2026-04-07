#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;


    printf("홀수 n의 값 : ");
    if (scanf("%d", &n) != 1 || n <= 0 || n % 2 == 0) {
        printf("3 이상의 홀수만 입력 가능\n");
        return 1;
    }

    
    int** arr = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
        *(arr + i) = (int*)malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++) {
            *(*(arr + i) + j) = 0;
        }
    }

    
    int r = 0;
    int c = n / 2;

    for (int num = 1; num <= n * n; num++) {
        *(*(arr + r) + c) = num;

        int next_r = r - 1;
        int next_c = c + 1;

        if (next_r < 0) next_r = n - 1;
        if (next_c >= n) next_c = 0;

        if (*(*(arr + next_r) + next_c) != 0) {
            r++;
        }
        else {
            r = next_r;
            c = next_c;
        }
    }

    
    printf("\n--- 생성된 %d x %d 마방진 ---\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", *(*(arr + i) + j));
        }
        printf("\n");
    }

    
    int magic_constant = n * (n * n + 1) / 2;
    
    
    printf("[가로 행(Rows) 합계]\n");
    for (int i = 0; i < n; i++) {
        int row_sum = 0;
        for (int j = 0; j < n; j++) {
            row_sum += *(*(arr + i) + j);
        }
        printf("  Row %d: %d\n", i + 1, row_sum);
    }

    
    printf("\n[세로 열(Columns) 합계]\n");
    for (int j = 0; j < n; j++) {
        int col_sum = 0;
        for (int i = 0; i < n; i++) {
            col_sum += *(*(arr + i) + j);
        }
        printf("  Col %d: %d\n", j + 1, col_sum);
    }

    
    int diag1_sum = 0; 
    int diag2_sum = 0; 
    for (int i = 0; i < n; i++) {
        diag1_sum += *(*(arr + i) + i);
        diag2_sum += *(*(arr + i) + (n - 1 - i));
    }
    printf("\n[대각선(Diagonals) 합계]\n");
    printf("  왼상단 대각선 (\\): %d\n", diag1_sum);
    printf("  우상단 대각선 (/): %d\n", diag2_sum);
    printf("========================================\n");

    
    for (int i = 0; i < n; i++) {
        free(*(arr + i));
    }
    free(arr);

    printf("메모리 해제 및 프로그램 종료\n");
    return 0;
}
