#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// 정렬 과정에서의 숫자 비교 횟수를 기록하는 전역 카운터
long long comparison_count = 0;

// 배열의 두 요소를 교환하는 함수
void swap_elements(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// [방법 1] 첫 번째 원소 선택 파티션
int partition_first(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low + 1;
    int j = high;

    while (1) {
        // 데이터 값과 피벗 값의 비교만 카운트 (인덱스 조건 검사는 제외)
        while (i <= high) {
            comparison_count++;
            if (arr[i] > pivot) break;
            i++;
        }
        while (j >= low + 1) {
            comparison_count++;
            if (arr[j] < pivot) break;
            j--;
        }
        if (i >= j) break;
        swap_elements(&arr[i], &arr[j]); // 데이터 이동(Swap)은 비교 횟수에 미포함
    }
    swap_elements(&arr[low], &arr[j]);
    return j;
}

// [방법 2] 무작위 원소 선택 파티션
int partition_random(int arr[], int low, int high) {
    // low부터 high 사이의 무작위 인덱스 선택
    int random_idx = low + rand() % (high - low + 1);
    swap_elements(&arr[low], &arr[random_idx]); // 난수 생성 및 Swap은 비교 횟수 미포함
    return partition_first(arr, low, high);
}

// [방법 3] 세 원소의 중앙값 선택 (Median-of-Three) 파티션
int partition_median_of_three(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;

    // 중앙값을 결정하기 위한 값 비교 과정 (규칙에 따라 카운트 포함)
    comparison_count++;
    if (arr[mid] < arr[low]) swap_elements(&arr[mid], &arr[low]);

    comparison_count++;
    if (arr[high] < arr[low]) swap_elements(&arr[high], &arr[low]);

    comparison_count++;
    if (arr[mid] < arr[high]) swap_elements(&arr[mid], &arr[high]);

    // 중앙값(mid 위치의 값)을 low로 이동
    swap_elements(&arr[low], &arr[mid]);
    return partition_first(arr, low, high);
}

// 퀵 정렬 재귀 함수
void quick_sort(int arr[], int low, int high, int mode) {
    if (low < high) {
        int pi = 0;
        if (mode == 1) {
            pi = partition_first(arr, low, high);
        }
        else if (mode == 2) {
            pi = partition_random(arr, low, high);
        }
        else if (mode == 3) {
            pi = partition_median_of_three(arr, low, high);
        }

        quick_sort(arr, low, pi - 1, mode);
        quick_sort(arr, pi + 1, high, mode);
    }
}

// 오름차순 정렬 여부 검증 함수
bool check_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

int main() {
    // 요구된 데이터 크기 목록
    int test_sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int num_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);

    for (int idx = 0; idx < num_sizes; idx++) {
        int n = test_sizes[idx];

        // 동적 배열 할당
        int* original_data = (int*)malloc(n * sizeof(int));
        int* data = (int*)malloc(n * sizeof(int));

        if (original_data == NULL || data == NULL) {
            printf("메모리 할당 실패\n");
            return 1;
        }

        // 동일한 무작위 데이터 생성 (시드 고정 및 중복 허용)
        srand(42 + n);
        for (int i = 0; i < n; i++) {
            original_data[i] = (rand() % 1000000) + 1;
        }

        long long comp1 = 0, comp2 = 0, comp3 = 0;

        // [방법 1] 첫 번째 원소 선택 테스트
        for (int i = 0; i < n; i++) data[i] = original_data[i];
        comparison_count = 0;
        quick_sort(data, 0, n - 1, 1);
        comp1 = comparison_count;
        if (!check_sorted(data, n)) printf("[오류] 방법 1 정렬 실패 (n=%d)\n", n);

        // [방법 2] 무작위 원소 선택 테스트
        for (int i = 0; i < n; i++) data[i] = original_data[i];
        comparison_count = 0;
        srand(12345); // 랜덤 피벗용 시드 고정
        quick_sort(data, 0, n - 1, 2);
        comp2 = comparison_count;
        if (!check_sorted(data, n)) printf("[오류] 방법 2 정렬 실패 (n=%d)\n", n);

        // [방법 3] 세 원소의 중앙값 선택 테스트
        for (int i = 0; i < n; i++) data[i] = original_data[i];
        comparison_count = 0;
        quick_sort(data, 0, n - 1, 3);
        comp3 = comparison_count;
        if (!check_sorted(data, n)) printf("[오류] 방법 3 정렬 실패 (n=%d)\n", n);

        // 요구된 출력 형식에 맞추어 출력
        printf("데이터 사이즈 : %d\n\n", n);
        printf("Quick Sort (First)    : %lld comparisons\n", comp1);
        printf("Quick Sort (Random)   : %lld comparisons\n", comp2);
        printf("Quick Sort (Median-3) : %lld comparisons\n\n", comp3);

        // 동적 메모리 해제
        free(original_data);
        free(data);
    }

    return 0;
}
