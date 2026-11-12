#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

// 재귀 호출 1단계당 보조 스택 프레임 크기 추정치 (인자, 지역변수, 복귀주소)
#define STACK_FRAME_BYTES 48

typedef struct {
    long long comparisons; // 데이터 값 비교 횟수
    long long movements;   // 데이터 값 복사 및 대입 횟수
    size_t current_mem;    // 현재 사용 중인 메모리 (Bytes)
    size_t peak_mem;       // 최대 메모리 사용량 (Bytes)
} Metrics;

typedef struct {
    int is_insert; // 1: 삽입(짝수), 0: 삭제(홀수)
    int value;     // 대상 정수 값
} Operation;

typedef struct AVLNode {
    int key;
    int count;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

// --- 고해상도 타이머 (ms 단위 반환) ---
double get_time_ms(void) {
#ifdef _WIN32
    static LARGE_INTEGER freq;
    static int initialized = 0;
    if (!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = 1;
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart * 1000.0 / (double)freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
#endif
}

// --- 메모리 사용량 추적 헬퍼 ---
static inline void mem_alloc(Metrics* m, size_t bytes) {
    m->current_mem += bytes;
    if (m->current_mem > m->peak_mem) {
        m->peak_mem = m->current_mem;
    }
}

static inline void mem_free(Metrics* m, size_t bytes) {
    if (m->current_mem >= bytes) {
        m->current_mem -= bytes;
    }
    else {
        m->current_mem = 0;
    }
}

// 30비트 무작위 양의 정수 생성 (플랫폼 무관 동일 범위 보장)
static inline int rand30(void) {
    return ((rand() & 0x7FFF) << 15) | (rand() & 0x7FFF);
}

// ============================================================================
// [1] 퀵 정렬 + 정렬 배열 선형 삽입·삭제
// ============================================================================

static inline void swap_int(int* a, int* b, Metrics* m) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
    m->movements += 3; // 데이터 값 대입 3회
}

// 삼중 중앙값(Median-of-Three) 피벗 분할
int partition(int* arr, int left, int right, Metrics* m) {
    int mid = left + (right - left) / 2;

    m->comparisons++;
    if (arr[left] > arr[mid]) swap_int(&arr[left], &arr[mid], m);
    m->comparisons++;
    if (arr[left] > arr[right]) swap_int(&arr[left], &arr[right], m);
    m->comparisons++;
    if (arr[mid] > arr[right]) swap_int(&arr[mid], &arr[right], m);

    if (right - left <= 2) return mid;

    swap_int(&arr[mid], &arr[right - 1], m);
    int pivot = arr[right - 1];
    int i = left;
    int j = right - 1;

    while (1) {
        while (1) {
            i++;
            m->comparisons++;
            if (arr[i] >= pivot) break;
        }
        while (1) {
            j--;
            m->comparisons++;
            if (arr[j] <= pivot) break;
        }
        if (i >= j) break;
        swap_int(&arr[i], &arr[j], m);
    }
    swap_int(&arr[i], &arr[right - 1], m);
    return i;
}

void quick_sort(int* arr, int left, int right, Metrics* m) {
    if (left >= right) return;

    mem_alloc(m, STACK_FRAME_BYTES);
    int p = partition(arr, left, right, m);
    quick_sort(arr, left, p - 1, m);
    quick_sort(arr, p + 1, right, m);
    mem_free(m, STACK_FRAME_BYTES);
}

void array_linear_insert(int* arr, int* size, int val, Metrics* m) {
    int i = 0;
    while (i < *size) {
        m->comparisons++;
        if (arr[i] >= val) break;
        i++;
    }
    for (int j = *size; j > i; j--) {
        arr[j] = arr[j - 1];
        m->movements++;
    }
    arr[i] = val;
    m->movements++;
    (*size)++;
}

void array_linear_delete(int* arr, int* size, int val, Metrics* m) {
    int i = 0;
    int found = 0;
    while (i < *size) {
        m->comparisons++;
        if (arr[i] >= val) {
            m->comparisons++;
            if (arr[i] == val) found = 1;
            break;
        }
        i++;
    }
    if (!found) return;

    for (int j = i; j < *size - 1; j++) {
        arr[j] = arr[j + 1];
        m->movements++;
    }
    (*size)--;
}

// ============================================================================
// [2] AVL 트리 정렬 + 동적 삽입·삭제 (포인터 변경은 데이터 이동에서 제외)
// ============================================================================

static inline int get_height(AVLNode* n) {
    return n ? n->height : 0;
}

static inline int max_int(int a, int b) {
    return (a > b) ? a : b;
}

static inline int get_balance(AVLNode* n) {
    return n ? get_height(n->left) - get_height(n->right) : 0;
}

AVLNode* create_avl_node(int key, Metrics* m) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    mem_alloc(m, sizeof(AVLNode));

    node->key = key;
    m->movements++; // 데이터 값 대입 1회
    node->count = 1;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 포인터 연결 변경만 수행하므로 m->movements는 증가시키지 않음
AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max_int(get_height(y->left), get_height(y->right)) + 1;
    x->height = max_int(get_height(x->left), get_height(x->right)) + 1;
    return x;
}

AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max_int(get_height(x->left), get_height(x->right)) + 1;
    y->height = max_int(get_height(y->left), get_height(y->right)) + 1;
    return y;
}

AVLNode* avl_insert(AVLNode* node, int key, Metrics* m) {
    if (node == NULL) {
        return create_avl_node(key, m);
    }

    mem_alloc(m, STACK_FRAME_BYTES);

    m->comparisons++;
    if (key == node->key) {
        node->count++;
        mem_free(m, STACK_FRAME_BYTES);
        return node;
    }

    m->comparisons++;
    if (key < node->key) {
        node->left = avl_insert(node->left, key, m);
    }
    else {
        node->right = avl_insert(node->right, key, m);
    }

    node->height = 1 + max_int(get_height(node->left), get_height(node->right));
    int balance = get_balance(node);

    // 균형 인수 비교는 데이터 값 비교가 아니므로 comparisons에 포함하지 않음
    if (balance > 1 && get_balance(node->left) >= 0) {
        node = rotate_right(node);
    }
    else if (balance > 1 && get_balance(node->left) < 0) {
        node->left = rotate_left(node->left);
        node = rotate_right(node);
    }
    else if (balance < -1 && get_balance(node->right) <= 0) {
        node = rotate_left(node);
    }
    else if (balance < -1 && get_balance(node->right) > 0) {
        node->right = rotate_right(node->right);
        node = rotate_left(node);
    }

    mem_free(m, STACK_FRAME_BYTES);
    return node;
}

AVLNode* min_value_node(AVLNode* node) {
    AVLNode* cur = node;
    while (cur->left != NULL) cur = cur->left;
    return cur;
}

AVLNode* avl_delete(AVLNode* root, int key, Metrics* m) {
    if (root == NULL) return NULL;

    mem_alloc(m, STACK_FRAME_BYTES);

    m->comparisons++;
    if (key == root->key) {
        if (root->count > 1) {
            root->count--;
            mem_free(m, STACK_FRAME_BYTES);
            return root;
        }

        if (root->left == NULL || root->right == NULL) {
            AVLNode* temp = root->left ? root->left : root->right;
            free(root);
            mem_free(m, sizeof(AVLNode));
            mem_free(m, STACK_FRAME_BYTES);
            return temp;
        }
        else {
            AVLNode* temp = min_value_node(root->right);
            root->key = temp->key;
            m->movements++; // 후속자 데이터 값 복사 1회
            root->count = temp->count;
            temp->count = 1;
            root->right = avl_delete(root->right, temp->key, m);
        }
    }
    else {
        m->comparisons++;
        if (key < root->key) {
            root->left = avl_delete(root->left, key, m);
        }
        else {
            root->right = avl_delete(root->right, key, m);
        }
    }

    root->height = 1 + max_int(get_height(root->left), get_height(root->right));
    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0) {
        root = rotate_right(root);
    }
    else if (balance > 1 && get_balance(root->left) < 0) {
        root->left = rotate_left(root->left);
        root = rotate_right(root);
    }
    else if (balance < -1 && get_balance(root->right) <= 0) {
        root = rotate_left(root);
    }
    else if (balance < -1 && get_balance(root->right) > 0) {
        root->right = rotate_right(root->right);
        root = rotate_left(root);
    }

    mem_free(m, STACK_FRAME_BYTES);
    return root;
}

// 중위 순회로 오름차순 결과 생성 및 데이터 이동 기록
void avl_inorder_collect(AVLNode* root, int* out, int* idx, Metrics* m) {
    if (!root) return;

    mem_alloc(m, STACK_FRAME_BYTES);
    avl_inorder_collect(root->left, out, idx, m);
    for (int i = 0; i < root->count; i++) {
        out[(*idx)++] = root->key;
        m->movements++; // 정렬 결과 배열로 데이터 값 복사
    }
    avl_inorder_collect(root->right, out, idx, m);
    mem_free(m, STACK_FRAME_BYTES);
}

int avl_total_elements(AVLNode* root) {
    if (!root) return 0;
    return root->count + avl_total_elements(root->left) + avl_total_elements(root->right);
}

void free_avl_tree(AVLNode* root) {
    if (!root) return;
    free_avl_tree(root->left);
    free_avl_tree(root->right);
    free(root);
}

// ============================================================================
// [3] 실험 실행 및 지정 형식 출력
// ============================================================================

void run_experiment(int n, unsigned int base_seed) {
    int k = n / 5; // 최초 데이터 크기의 20%
    int value_range = n; // 중복 허용 및 유효 삭제 발생을 위한 난수 범위

    // 1. 원본 데이터 및 연산 시퀀스 사전 생성 (시간 측정 제외)
    int* origin = (int*)malloc(sizeof(int) * n);
    Operation* ops = (Operation*)malloc(sizeof(Operation) * k);

    srand(base_seed);
    for (int i = 0; i < n; i++) {
        origin[i] = rand30() % value_range;
    }

    srand(base_seed + 9999);
    for (int i = 0; i < k; i++) {
        ops[i].is_insert = (rand() % 2 == 0) ? 1 : 0; // 짝수: 삽입, 홀수: 삭제
        ops[i].value = rand30() % value_range;        // 별도 난수로 값 생성
    }

    // 2. Quick Sort 실험 (초기 정렬 + 삽입/삭제)
    Metrics qs_m = { 0, 0, 0, 0 };
    size_t qs_capacity_bytes = sizeof(int) * (size_t)(n + k);
    int* qs_arr = (int*)malloc(qs_capacity_bytes);
    memcpy(qs_arr, origin, sizeof(int) * n);
    int qs_size = n;
    mem_alloc(&qs_m, qs_capacity_bytes);

    double qs_start = get_time_ms();
    quick_sort(qs_arr, 0, qs_size - 1, &qs_m);
    for (int i = 0; i < k; i++) {
        if (ops[i].is_insert) {
            array_linear_insert(qs_arr, &qs_size, ops[i].value, &qs_m);
        }
        else {
            array_linear_delete(qs_arr, &qs_size, ops[i].value, &qs_m);
        }
    }
    double qs_end = get_time_ms();
    double qs_time = qs_end - qs_start;

    // 3. AVL Tree Sort 실험 (초기 삽입 + 중위 순회 확인 + 삽입/삭제)
    Metrics avl_m = { 0, 0, 0, 0 };
    AVLNode* root = NULL;

    double avl_start = get_time_ms();
    for (int i = 0; i < n; i++) {
        root = avl_insert(root, origin[i], &avl_m);
    }

    // 중위 순회로 오름차순 결과 확인
    size_t inorder_buf_bytes = sizeof(int) * (size_t)n;
    int* sorted_check = (int*)malloc(inorder_buf_bytes);
    mem_alloc(&avl_m, inorder_buf_bytes);
    int check_idx = 0;
    avl_inorder_collect(root, sorted_check, &check_idx, &avl_m);
    free(sorted_check);
    mem_free(&avl_m, inorder_buf_bytes);

    // 동적 삽입·삭제 수행
    for (int i = 0; i < k; i++) {
        if (ops[i].is_insert) {
            root = avl_insert(root, ops[i].value, &avl_m);
        }
        else {
            root = avl_delete(root, ops[i].value, &avl_m);
        }
    }
    double avl_end = get_time_ms();
    double avl_time = avl_end - avl_start;

    // 4. 최종 데이터 구성 일치 검증 (시간 측정 제외)
    int avl_size = avl_total_elements(root);
    int* final_avl_arr = (int*)malloc(sizeof(int) * (avl_size > 0 ? avl_size : 1));
    int final_idx = 0;
    Metrics dummy_m = { 0, 0, 0, 0 };
    avl_inorder_collect(root, final_avl_arr, &final_idx, &dummy_m);
    assert(qs_size == avl_size);
    assert(memcmp(qs_arr, final_avl_arr, sizeof(int) * qs_size) == 0);
    free(final_avl_arr);

    // 5. 지정된 출력 포맷에 맞춰 결과 출력
    char qs_time_str[32], avl_time_str[32];
    char qs_comp_str[32], avl_comp_str[32];
    char qs_move_str[32], avl_move_str[32];
    char qs_mem_str[32], avl_mem_str[32];
    char qs_cnt_str[32], avl_cnt_str[32];

    snprintf(qs_time_str, sizeof(qs_time_str), "%.3f ms", qs_time);
    snprintf(avl_time_str, sizeof(avl_time_str), "%.3f ms", avl_time);
    snprintf(qs_comp_str, sizeof(qs_comp_str), "%lld", qs_m.comparisons);
    snprintf(avl_comp_str, sizeof(avl_comp_str), "%lld", avl_m.comparisons);
    snprintf(qs_move_str, sizeof(qs_move_str), "%lld", qs_m.movements);
    snprintf(avl_move_str, sizeof(avl_move_str), "%lld", avl_m.movements);
    snprintf(qs_mem_str, sizeof(qs_mem_str), "%zu Bytes", qs_m.peak_mem);
    snprintf(avl_mem_str, sizeof(avl_mem_str), "%zu Bytes", avl_m.peak_mem);
    snprintf(qs_cnt_str, sizeof(qs_cnt_str), "%d", qs_size);
    snprintf(avl_cnt_str, sizeof(avl_cnt_str), "%d", avl_size);

    printf("Data Size : %d\n", n);
    printf("Operations: %d\n\n", k);
    printf("%-18s%-16s%s\n", "", "Quick Sort", "AVL Tree");
    printf("------------------------------------------------\n");
    printf("%-18s%-16s%s\n", "Total Time", qs_time_str, avl_time_str);
    printf("%-18s%-16s%s\n", "Comparisons", qs_comp_str, avl_comp_str);
    printf("%-18s%-16s%s\n", "Data Movements", qs_move_str, avl_move_str);
    printf("%-18s%-16s%s\n\n", "Memory Usage", qs_mem_str, avl_mem_str);
    printf("%-18s%-16s%s\n\n\n", "Final Data Count", qs_cnt_str, avl_cnt_str);

    // 자원 해제
    free(origin);
    free(ops);
    free(qs_arr);
    free_avl_tree(root);
}

int main(void) {
    const int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    const int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    const unsigned int SEED = 20261112;

    for (int i = 0; i < num_sizes; i++) {
        run_experiment(sizes[i], SEED + (unsigned int)sizes[i]);
    }

    return 0;
}
