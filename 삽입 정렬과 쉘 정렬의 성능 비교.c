#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 정렬 유효성 검증 함수 (오름차순 정렬 확인)
int is_sorted(const int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// [1] 삽입 정렬 (Insertion Sort)
void insertion_sort(int arr[], int n, long long* compare_count) {
    *compare_count = 0;
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            (*compare_count)++; // 순수 데이터 값 비교 횟수 측정
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

// [2] 쉘 정렬 - 기본 간격 (Shell's Original Sequence: h = n/2, n/4, ..., 1)
void shell_sort_original(int arr[], int n, long long* compare_count) {
    *compare_count = 0;
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int key = arr[i];
            int j = i;
            while (j >= gap) {
                (*compare_count)++; // 순수 데이터 값 비교 횟수 측정
                if (arr[j - gap] > key) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                else {
                    break;
                }
            }
            arr[j] = key;
        }
    }
}

// [3] 쉘 정렬 - Knuth 간격 수열 (h_1 = 1, h_{k+1} = 3*h_k + 1)
void shell_sort_knuth(int arr[], int n, long long* compare_count) {
    *compare_count = 0;

    // 데이터 크기 n보다 작은 가장 큰 Knuth 간격 산출
    int gap = 1;
    while (gap < n) {
        gap = 3 * gap + 1; // 1, 4, 13, 40, 121, 364, 1093, ...
    }
    gap = (gap - 1) / 3; // n보다 작은 최대 간격으로 보정

    // 역순으로 간격 축소 적용 (마지막 gap = 1)
    for (; gap > 0; gap = (gap - 1) / 3) {
        for (int i = gap; i < n; i++) {
            int key = arr[i];
            int j = i;
            while (j >= gap) {
                (*compare_count)++; // 순수 데이터 값 비교 횟수 측정
                if (arr[j - gap] > key) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                else {
                    break;
                }
            }
            arr[j] = key;
        }
    }
}

// 원본 데이터 복사 함수
void copy_array(const int src[], int dest[], int n) {
    memcpy(dest, src, sizeof(int) * n);
}

// 데이터 크기별 실험 수행 및 한국어 양식 출력 함수
void run_experiment(int n) {
    int* original = (int*)malloc(sizeof(int) * n);
    int* test_arr = (int*)malloc(sizeof(int) * n);
    long long cmp_insertion = 0, cmp_shell_orig = 0, cmp_shell_knuth = 0;

    // 난수 데이터 생성 (동일 조건 보장)
    for (int i = 0; i < n; i++) {
        original[i] = (rand() << 15 | rand()) % (n * 10);
    }

    // [1] 삽입 정렬
    copy_array(original, test_arr, n);
    insertion_sort(test_arr, n, &cmp_insertion);

    // [2] 쉘 정렬 - 기본 간격
    copy_array(original, test_arr, n);
    shell_sort_original(test_arr, n, &cmp_shell_orig);

    // [3] 쉘 정렬 - Knuth 간격
    copy_array(original, test_arr, n);
    shell_sort_knuth(test_arr, n, &cmp_shell_knuth);

    // 한국어 출력 양식 적용
    printf("데이터 크기 : %d\n\n", n);
    printf("삽입 정렬             : %lld회 비교\n", cmp_insertion);
    printf("쉘 정렬 (기본 간격)   : %lld회 비교\n", cmp_shell_orig);
    printf("쉘 정렬 (Knuth 간격)  : %lld회 비교\n\n", cmp_shell_knuth);

    free(original);
    free(test_arr);
}

int main() {
    srand((unsigned int)time(NULL));

    int test_sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);

    for (int i = 0; i < num_tests; i++) {
        run_experiment(test_sizes[i]);
    }

    return 0;
}
