#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define STACK_FRAME_SIZE 48 

long long cmp_count = 0;
int current_depth = 0;
int max_depth = 0;

void reset_metrics() {
    cmp_count = 0;
    current_depth = 0;
    max_depth = 0;
}

void track_depth() {
    current_depth++;
    if (current_depth > max_depth) max_depth = current_depth;
}

void untrack_depth() {
    current_depth--;
}

void swap(int* a, int* b) {
    int t = *a; *a = *b; *b = t;
}

// ==========================================
// [1] 병합 정렬 (Merge Sort)
// ==========================================
void merge(int arr[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    int* temp = (int*)malloc(sizeof(int) * (right + 1));

    while (i <= mid && j <= right) {
        cmp_count++;
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (int l = left; l <= right; l++) arr[l] = temp[l];

    free(temp);
}

void merge_sort(int arr[], int left, int right) {
    track_depth();
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
    untrack_depth();
}

// ==========================================
// [2] 퀵 정렬 (Quick Sort) 파티션
// ==========================================
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        cmp_count++;
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// 2-1. First Element
void quick_sort_first(int arr[], int low, int high) {
    track_depth();
    if (low < high) {
        swap(&arr[low], &arr[high]);
        int pi = partition(arr, low, high);
        quick_sort_first(arr, low, pi - 1);
        quick_sort_first(arr, pi + 1, high);
    }
    untrack_depth();
}

// 2-2. Random Pivot
void quick_sort_random(int arr[], int low, int high) {
    track_depth();
    if (low < high) {
        int random_idx = low + rand() % (high - low + 1);
        swap(&arr[random_idx], &arr[high]);
        int pi = partition(arr, low, high);
        quick_sort_random(arr, low, pi - 1);
        quick_sort_random(arr, pi + 1, high);
    }
    untrack_depth();
}

// 2-3. Median-of-Three
void quick_sort_median(int arr[], int low, int high) {
    track_depth();
    if (low < high) {
        int mid = low + (high - low) / 2;

        cmp_count++;
        if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);
        cmp_count++;
        if (arr[mid] > arr[high]) swap(&arr[mid], &arr[high]);
        cmp_count++;
        if (arr[low] > arr[mid]) swap(&arr[low], &arr[mid]);

        swap(&arr[mid], &arr[high]);
        int pi = partition(arr, low, high);
        quick_sort_median(arr, low, pi - 1);
        quick_sort_median(arr, pi + 1, high);
    }
    untrack_depth();
}

// ==========================================
// 벤치마크 실행 및 서식 출력부
// ==========================================
void run_test(const char* name, void (*sort_func)(int[], int, int), int arr[], int size, int is_merge) {
    reset_metrics();

    clock_t start = clock();
    sort_func(arr, 0, size - 1);
    clock_t end = clock();

    // 밀리초(ms) 단위 변환
    double time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    long long stack_memory = max_depth * STACK_FRAME_SIZE;
    long long heap_memory = is_merge ? (size * sizeof(int)) : 0;
    long long total_memory = stack_memory + heap_memory;

    // 요구된 폭(Width)에 맞춘 서식 지정자 출력
    printf("%-15s %-14lld %-11.3f %lld\n", name, cmp_count, time_ms, total_memory);
}

int main() {
    srand((unsigned int)time(NULL));
    int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];

        // 원본 배열 생성
        int* original = (int*)malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++) {
            original[j] = rand() % 100000;
        }

        printf("Data Size : %d\n\n", n);
        printf("%-15s %-14s %-11s %s\n", "Algorithm", "Comparisons", "Time(ms)", "Memory(Bytes)");
        printf("--------------------------------------------------------\n");

        int* arr_test = (int*)malloc(sizeof(int) * n);

        // 1. 병합 정렬
        memcpy(arr_test, original, sizeof(int) * n);
        run_test("Merge Sort", merge_sort, arr_test, n, 1);

        // 2. 퀵 정렬 (First)
        memcpy(arr_test, original, sizeof(int) * n);
        run_test("Quick (First)", quick_sort_first, arr_test, n, 0);

        // 3. 퀵 정렬 (Random)
        memcpy(arr_test, original, sizeof(int) * n);
        run_test("Quick (Random)", quick_sort_random, arr_test, n, 0);

        // 4. 퀵 정렬 (Median3)
        memcpy(arr_test, original, sizeof(int) * n);
        run_test("Quick (Median3)", quick_sort_median, arr_test, n, 0);

        printf("\n");

        free(original);
        free(arr_test);
    }

    return 0;
}
