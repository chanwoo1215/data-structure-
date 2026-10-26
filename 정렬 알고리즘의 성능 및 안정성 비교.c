#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define DATA_COUNT 10000
#define NUM_TRIALS 100  // 필요시 테스트 단계에서는 10으로 줄여서 확인 가능

typedef struct {
    int key1;
    int key2;
    int original_index;
} Data;

void format_number(long long val, char* out) {
    char buf[64];
    sprintf(buf, "%lld", val);
    int len = (int)strlen(buf);
    int out_idx = 0;

    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) {
            out[out_idx++] = ',';
        }
        out[out_idx++] = buf[i];
    }
    out[out_idx] = '\0';
}

int get_rand_1M(void) {
    long long r = ((long long)rand() << 15) | rand();
    return (int)(r % 1000001);
}

void copy_dataset(const Data* src, Data* dest, int n) {
    memcpy(dest, src, sizeof(Data) * n);
}

int verify_sorted(const Data* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i].key1 > arr[i + 1].key1) return 0;
    }
    return 1;
}

int check_stability(const Data* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i].key1 == arr[i + 1].key1) {
            if (arr[i].original_index > arr[i + 1].original_index) return 0;
        }
    }
    return 1;
}

void selection_sort(const Data* src, Data* dest, int n, long long* comps, long long* moves) {
    copy_dataset(src, dest, n);
    *comps = 0; *moves = 0;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            (*comps)++;
            if (dest[j].key1 < dest[min_idx].key1) min_idx = j;
        }
        if (min_idx != i) {
            Data temp = dest[i];
            dest[i] = dest[min_idx];
            dest[min_idx] = temp;
            (*moves) += 3;
        }
    }
}

void bubble_sort_standard(const Data* src, Data* dest, int n, long long* comps, long long* moves) {
    copy_dataset(src, dest, n);
    *comps = 0; *moves = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comps)++;
            if (dest[j].key1 > dest[j + 1].key1) {
                Data temp = dest[j];
                dest[j] = dest[j + 1];
                dest[j + 1] = temp;
                (*moves) += 3;
            }
        }
    }
}

void bubble_sort_early(const Data* src, Data* dest, int n, long long* comps, long long* moves) {
    copy_dataset(src, dest, n);
    *comps = 0; *moves = 0;
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comps)++;
            if (dest[j].key1 > dest[j + 1].key1) {
                Data temp = dest[j];
                dest[j] = dest[j + 1];
                dest[j + 1] = temp;
                (*moves) += 3;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

void insertion_sort_case_a(const Data* src, Data* dest, int n, long long* comps, long long* moves) {
    copy_dataset(src, dest, n);
    *comps = 0; *moves = 0;
    for (int i = 1; i < n; i++) {
        Data key_data = dest[i];
        (*moves)++;
        int j = i - 1;
        while (j >= 0) {
            (*comps)++;
            if (dest[j].key1 > key_data.key1) {
                dest[j + 1] = dest[j];
                (*moves)++;
                j--;
            }
            else break;
        }
        dest[j + 1] = key_data;
        (*moves)++;
    }
}

void insertion_sort_case_b(const Data* src, Data* dest, int n, long long* comps, long long* moves) {
    copy_dataset(src, dest, n);
    *comps = 0; *moves = 0;
    for (int i = 1; i < n; i++) {
        Data key_data = dest[i];
        (*moves)++;
        int j = i - 1;
        while (j >= 0) {
            (*comps)++;
            if (dest[j].key1 >= key_data.key1) {
                dest[j + 1] = dest[j];
                (*moves)++;
                j--;
            }
            else break;
        }
        dest[j + 1] = key_data;
        (*moves)++;
    }
}

int compare_asc(const void* a, const void* b) {
    const Data* d1 = (const Data*)a;
    const Data* d2 = (const Data*)b;
    if (d1->key1 < d2->key1) return -1;
    if (d1->key1 > d2->key1) return 1;
    return 0;
}

int compare_desc(const void* a, const void* b) {
    const Data* d1 = (const Data*)a;
    const Data* d2 = (const Data*)b;
    if (d1->key1 > d2->key1) return -1;
    if (d1->key1 < d2->key1) return 1;
    return 0;
}

void generate_random_data(Data* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i].key1 = get_rand_1M();
        arr[i].key2 = get_rand_1M();
        arr[i].original_index = i;
    }
}

void generate_stability_data(Data* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i].key1 = rand() % 500;
        arr[i].key2 = get_rand_1M();
        arr[i].original_index = i;
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    Data* raw_data = (Data*)malloc(sizeof(Data) * DATA_COUNT);
    Data* result_data = (Data*)malloc(sizeof(Data) * DATA_COUNT);

    Data* sorted_base = (Data*)malloc(sizeof(Data) * DATA_COUNT);
    Data* reverse_base = (Data*)malloc(sizeof(Data) * DATA_COUNT);
    Data* partially_base = (Data*)malloc(sizeof(Data) * DATA_COUNT);

    char fmt1[64], fmt2[64], fmt3[64];

    printf("Data Count : %d\n\n", DATA_COUNT);

    long long total_c_sel = 0, total_m_sel = 0;
    long long total_c_bub = 0, total_m_bub = 0;
    long long total_c_ear = 0, total_m_ear = 0;
    long long total_c_ins = 0, total_m_ins = 0;

    printf("Running Random Data Experiments (%d trials)...\n", NUM_TRIALS);
    fflush(stdout);

    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        generate_random_data(raw_data, DATA_COUNT);
        long long c, m;

        selection_sort(raw_data, result_data, DATA_COUNT, &c, &m);
        total_c_sel += c; total_m_sel += m;

        bubble_sort_standard(raw_data, result_data, DATA_COUNT, &c, &m);
        total_c_bub += c; total_m_bub += m;

        bubble_sort_early(raw_data, result_data, DATA_COUNT, &c, &m);
        total_c_ear += c; total_m_ear += m;

        insertion_sort_case_a(raw_data, result_data, DATA_COUNT, &c, &m);
        total_c_ins += c; total_m_ins += m;

        // 10회마다 진행 상황 출력 및 버퍼 즉시 출력
        if ((trial + 1) % 10 == 0 || trial == NUM_TRIALS - 1) {
            printf("Progress: %d / %d completed...\n", trial + 1, NUM_TRIALS);
            fflush(stdout);
        }
    }

    printf("\nRandom Data\n");
    printf("Number of Trials : %d\n\n", NUM_TRIALS);

    format_number(total_c_sel, fmt1);
    format_number(total_c_sel / NUM_TRIALS, fmt2);
    format_number(total_m_sel / NUM_TRIALS, fmt3);
    printf("Selection Sort\nTotal Comparisons   : %s\nAverage Comparisons : %s\nAverage Movements   : %s\n\n", fmt1, fmt2, fmt3);

    format_number(total_c_bub, fmt1);
    format_number(total_c_bub / NUM_TRIALS, fmt2);
    format_number(total_m_bub / NUM_TRIALS, fmt3);
    printf("Bubble Sort\nTotal Comparisons   : %s\nAverage Comparisons : %s\nAverage Movements   : %s\n\n", fmt1, fmt2, fmt3);

    format_number(total_c_ear, fmt1);
    format_number(total_c_ear / NUM_TRIALS, fmt2);
    format_number(total_m_ear / NUM_TRIALS, fmt3);
    printf("Bubble Sort (Early Termination)\nTotal Comparisons   : %s\nAverage Comparisons : %s\nAverage Movements   : %s\n\n", fmt1, fmt2, fmt3);

    format_number(total_c_ins, fmt1);
    format_number(total_c_ins / NUM_TRIALS, fmt2);
    format_number(total_m_ins / NUM_TRIALS, fmt3);
    printf("Insertion Sort\nTotal Comparisons   : %s\nAverage Comparisons : %s\nAverage Movements   : %s\n", fmt1, fmt2, fmt3);

    printf("\n------------------------------------\n\n");

    generate_random_data(raw_data, DATA_COUNT);

    copy_dataset(raw_data, sorted_base, DATA_COUNT);
    qsort(sorted_base, DATA_COUNT, sizeof(Data), compare_asc);

    copy_dataset(raw_data, reverse_base, DATA_COUNT);
    qsort(reverse_base, DATA_COUNT, sizeof(Data), compare_desc);

    copy_dataset(sorted_base, partially_base, DATA_COUNT);
    int swap_count = (int)(DATA_COUNT * 0.05);
    for (int k = 0; k < swap_count; k++) {
        int idx1 = rand() % DATA_COUNT;
        int idx2 = rand() % DATA_COUNT;
        Data temp = partially_base[idx1];
        partially_base[idx1] = partially_base[idx2];
        partially_base[idx2] = temp;
    }

    long long comps, moves;

    printf("Sorted Data\n\n");

    selection_sort(sorted_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Selection Sort\nComparisons : %s\nMovements   : %s\n\n", fmt1, fmt2);

    bubble_sort_standard(sorted_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Bubble Sort\nComparisons : %s\nMovements   : %s\n\n", fmt1, fmt2);

    bubble_sort_early(sorted_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Bubble Sort (Early Termination)\nComparisons : %s\nMovements   : %s\n\n", fmt1, fmt2);

    insertion_sort_case_a(sorted_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Insertion Sort\nComparisons : %s\nMovements   : %s\n", fmt1, fmt2);

    printf("\n------------------------------------\n\n");

    printf("Reverse-Sorted Data\n\n");

    selection_sort(reverse_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Selection Sort\nComparisons : %s\nMovements   : %s\n\n", fmt1, fmt2);

    bubble_sort_standard(reverse_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Bubble Sort\nComparisons : %s\nMovements   : %s\n\n", fmt1, fmt2);

    bubble_sort_early(reverse_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Bubble Sort (Early Termination)\nComparisons : %s\nMovements   : %s\n\n", fmt1, fmt2);

    insertion_sort_case_a(reverse_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Insertion Sort\nComparisons : %s\nMovements   : %s\n", fmt1, fmt2);

    printf("\n------------------------------------\n\n");

    printf("Partially Sorted Data\n\n");

    selection_sort(partially_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Selection Sort\nComparisons : %s\nMovements   : %s\n\n", fmt1, fmt2);

    bubble_sort_standard(partially_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Bubble Sort\nComparisons : %s\nMovements   : %s\n\n", fmt1, fmt2);

    bubble_sort_early(partially_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Bubble Sort (Early Termination)\nComparisons : %s\nMovements   : %s\n\n", fmt1, fmt2);

    insertion_sort_case_a(partially_base, result_data, DATA_COUNT, &comps, &moves);
    format_number(comps, fmt1); format_number(moves, fmt2);
    printf("Insertion Sort\nComparisons : %s\nMovements   : %s\n", fmt1, fmt2);

    printf("\n------------------------------------\n\n");

    generate_stability_data(raw_data, DATA_COUNT);

    printf("Stability Verification\n\n");

    selection_sort(raw_data, result_data, DATA_COUNT, &comps, &moves);
    printf("Selection Sort       : %s\n", check_stability(result_data, DATA_COUNT) ? "Stable" : "Unstable");

    bubble_sort_standard(raw_data, result_data, DATA_COUNT, &comps, &moves);
    printf("Bubble Sort          : %s\n", check_stability(result_data, DATA_COUNT) ? "Stable" : "Unstable");

    insertion_sort_case_a(raw_data, result_data, DATA_COUNT, &comps, &moves);
    printf("Insertion Sort (>)   : %s\n", check_stability(result_data, DATA_COUNT) ? "Stable" : "Unstable");

    insertion_sort_case_b(raw_data, result_data, DATA_COUNT, &comps, &moves);
    printf("Insertion Sort (>=)  : %s\n", check_stability(result_data, DATA_COUNT) ? "Stable" : "Unstable");

    free(raw_data); free(result_data);
    free(sorted_base); free(reverse_base); free(partially_base);

    return 0;
}
