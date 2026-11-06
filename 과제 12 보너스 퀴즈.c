#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// 배열의 두 요소를 교환하는 함수
void swap_elements(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// [방법 3] 세 원소의 중앙값 선택 (Median-of-Three) 파티션
// (두 정렬 방식이 동일한 피벗 선택 및 분할 로직을 공유하도록 분리)
int partition_median_of_three(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;

    if (arr[mid] < arr[low]) swap_elements(&arr[mid], &arr[low]);
    if (arr[high] < arr[low]) swap_elements(&arr[high], &arr[low]);
    if (arr[mid] < arr[high]) swap_elements(&arr[mid], &arr[high]);

    swap_elements(&arr[low], &arr[mid]);

    int pivot = arr[low];
    int i = low + 1;
    int j = high;

    while (1) {
        while (i <= high) {
            if (arr[i] > pivot) break;
            i++;
        }
        while (j >= low + 1) {
            if (arr[j] < pivot) break;
            j--;
        }
        if (i >= j) break;
        swap_elements(&arr[i], &arr[j]);
    }
    swap_elements(&arr[low], &arr[j]);
    return j;
}

// 1. 재귀적 퀵 정렬 (Recursive)
void quick_sort_recursive(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition_median_of_three(arr, low, high);
        quick_sort_recursive(arr, low, pi - 1);
        quick_sort_recursive(arr, pi + 1, high);
    }
}

// 스택 프레임 구조체 정의 (반복적 퀵 정렬용)
typedef struct {
    int low;
    int high;
} StackFrame;

// 2. 반복적 퀵 정렬 (Iterative - 명시적 스택 활용)
void quick_sort_iterative(int arr[], int low, int high) {
    int size = high - low + 1;
    StackFrame* stack = (StackFrame*)malloc(size * sizeof(StackFrame));
    int top = -1;

    // 초기 범위를 스택에 푸시
    stack[++top] = (StackFrame){ low, high };

    while (top >= 0) {
        StackFrame frame = stack[top--];
        int l = frame.low;
        int h = frame.high;

        if (l < h) {
            int pi = partition_median_of_three(arr, l, h);

            // 좌측 구간과 우측 구간을 스택에 푸시
            stack[++top] = (StackFrame){ l, pi - 1 };
            stack[++top] = (StackFrame){ pi + 1, h };
        }
    }
    free(stack);
}

// 오름차순 정렬 여부 검증 함수
bool check_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

int main() {
    int n = 10000;
    int iterations = 1000; // 실행 시간이 매우 짧으므로 1,000회 반복 측정 후 평균 산출

    int* original_data = (int*)malloc(n * sizeof(int));
    int* data_rec = (int*)malloc(n * sizeof(int));
    int* data_iter = (int*)malloc(n * sizeof(int));

    if (!original_data || !data_rec || !data_iter) {
        printf("메모리 할당 실패\n");
        return 1;
    }

    // 동일한 원본 무작위 데이터 생성
    srand(42);
    for (int i = 0; i < n; i++) {
        original_data[i] = (rand() % 1000000) + 1;
    }

    // --- 재귀적 퀵 정렬 실행 시간 측정 ---
    clock_t total_rec_time = 0;
    for (int iter = 0; iter < iterations; iter++) {
        for (int i = 0; i < n; i++) data_rec[i] = original_data[i]; // 복사는 측정에서 제외하기 위해 분리

        clock_t t1 = clock();
        quick_sort_recursive(data_rec, 0, n - 1);
        clock_t t2 = clock();
        total_rec_time += (t2 - t1);
    }

    if (!check_sorted(data_rec, n)) printf("[오류] 재귀적 정렬 검증 실패\n");

    // --- 반복적 퀵 정렬 실행 시간 측정 ---
    clock_t total_iter_time = 0;
    for (int iter = 0; iter < iterations; iter++) {
        for (int i = 0; i < n; i++) data_iter[i] = original_data[i]; // 복사는 측정에서 제외

        clock_t t1 = clock();
        quick_sort_iterative(data_iter, 0, n - 1);
        clock_t t2 = clock();
        total_iter_time += (t2 - t1);
    }

    if (!check_sorted(data_iter, n)) printf("[오류] 반복적 정렬 검증 실패\n");

    // 밀리초(ms) 단위로 평균 실행 시간 계산
    double avg_rec_ms = ((double)total_rec_time / CLOCKS_PER_SEC * 1000.0) / iterations;
    double avg_iter_ms = ((double)total_iter_time / CLOCKS_PER_SEC * 1000.0) / iterations;

    // 요구된 출력 형식
    printf("Data Size : %d\n", n);
    printf("Pivot     : Median-of-Three\n\n");
    printf("Recursive Quick Sort : %.4f ms\n", avg_rec_ms);
    printf("Iterative Quick Sort : %.4f ms\n", avg_iter_ms);

    // 메모리 해제
    free(original_data);
    free(data_rec);
    free(data_iter);

    return 0;
}
