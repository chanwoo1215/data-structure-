#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VAL 1000
#define INSERT_COUNT 100
#define SEARCH_COUNT 50

// ==========================================
// 1. 정수 배열 구조체 및 함수
// ==========================================
typedef struct {
    int data[INSERT_COUNT];
    int size;
    long long insert_compare_count;
    long long total_search_compare_count;
} ArrayStructure;

void init_array(ArrayStructure* arr) {
    arr->size = 0;
    arr->insert_compare_count = 0;
    arr->total_search_compare_count = 0;
}

void insert_array(ArrayStructure* arr, int val) {
    int exists = 0;
    for (int i = 0; i < arr->size; i++) {
        arr->insert_compare_count++;
        if (arr->data[i] == val) {
            exists = 1;
            break;
        }
    }
    if (!exists && arr->size < INSERT_COUNT) {
        arr->data[arr->size++] = val;
    }
}

int search_array(ArrayStructure* arr, int target, int* compare_count) {
    *compare_count = 0;
    for (int i = 0; i < arr->size; i++) {
        (*compare_count)++;
        if (arr->data[i] == target) {
            return 1; // 탐색 성공
        }
    }
    return 0; // 탐색 실패
}

// ==========================================
// 2. 이진 탐색 트리(BST) 구조체 및 함수
// ==========================================
typedef struct BSTNode {
    int key;
    struct BSTNode* left, * right;
} BSTNode;

typedef struct {
    BSTNode* root;
    int node_count;
    long long insert_compare_count;
    long long total_search_compare_count;
} BSTStructure;

void init_bst(BSTStructure* tree) {
    tree->root = NULL;
    tree->node_count = 0;
    tree->insert_compare_count = 0;
    tree->total_search_compare_count = 0;
}

BSTNode* insert_bst_node(BSTNode* node, int key, long long* compare_count, int* is_inserted) {
    if (node == NULL) {
        *is_inserted = 1;
        BSTNode* new_node = (BSTNode*)malloc(sizeof(BSTNode));
        new_node->key = key;
        new_node->left = new_node->right = NULL;
        return new_node;
    }

    (*compare_count)++;
    if (key == node->key) {
        *is_inserted = 0;
        return node;
    }
    else if (key < node->key) {
        node->left = insert_bst_node(node->left, key, compare_count, is_inserted);
    }
    else {
        node->right = insert_bst_node(node->right, key, compare_count, is_inserted);
    }
    return node;
}

void insert_bst(BSTStructure* tree, int val) {
    int is_inserted = 0;
    tree->root = insert_bst_node(tree->root, val, &(tree->insert_compare_count), &is_inserted);
    if (is_inserted) tree->node_count++;
}

int search_bst(BSTStructure* tree, int target, int* compare_count) {
    *compare_count = 0;
    BSTNode* curr = tree->root;
    while (curr != NULL) {
        (*compare_count)++;
        if (curr->key == target) {
            return 1;
        }
        else if (target < curr->key) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    return 0;
}

int get_bst_height(const BSTNode* node) {
    if (node == NULL) return 0;
    int left_h = get_bst_height(node->left);
    int right_h = get_bst_height(node->right);
    return (left_h > right_h ? left_h : right_h) + 1;
}

// ==========================================
// 3. AVL 트리 구조체 및 함수
// ==========================================
typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode* left, * right;
} AVLNode;

typedef struct {
    AVLNode* root;
    int node_count;
    long long insert_compare_count;
    long long total_search_compare_count;
} AVLStructure;

void init_avl(AVLStructure* tree) {
    tree->root = NULL;
    tree->node_count = 0;
    tree->insert_compare_count = 0;
    tree->total_search_compare_count = 0;
}

int get_avl_node_height(AVLNode* n) { return (n == NULL) ? 0 : n->height; }
int max_val(int a, int b) { return (a > b) ? a : b; }

AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max_val(get_avl_node_height(y->left), get_avl_node_height(y->right)) + 1;
    x->height = max_val(get_avl_node_height(x->left), get_avl_node_height(x->right)) + 1;
    return x;
}

AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max_val(get_avl_node_height(x->left), get_avl_node_height(x->right)) + 1;
    y->height = max_val(get_avl_node_height(y->left), get_avl_node_height(y->right)) + 1;
    return y;
}

int get_balance_factor(AVLNode* n) {
    return (n == NULL) ? 0 : get_avl_node_height(n->left) - get_avl_node_height(n->right);
}

AVLNode* insert_avl_node(AVLNode* node, int key, long long* compare_count, int* is_inserted) {
    if (node == NULL) {
        *is_inserted = 1;
        AVLNode* new_node = (AVLNode*)malloc(sizeof(AVLNode));
        new_node->key = key;
        new_node->height = 1;
        new_node->left = new_node->right = NULL;
        return new_node;
    }

    (*compare_count)++;
    if (key == node->key) {
        *is_inserted = 0;
        return node;
    }
    else if (key < node->key) {
        node->left = insert_avl_node(node->left, key, compare_count, is_inserted);
    }
    else {
        node->right = insert_avl_node(node->right, key, compare_count, is_inserted);
    }

    if (!(*is_inserted)) return node;

    node->height = 1 + max_val(get_avl_node_height(node->left), get_avl_node_height(node->right));
    int balance = get_balance_factor(node);

    if (balance > 1 && key < node->left->key) return rotate_right(node);
    if (balance < -1 && key > node->right->key) return rotate_left(node);
    if (balance > 1 && key > node->left->key) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void insert_avl(AVLStructure* tree, int val) {
    int is_inserted = 0;
    tree->root = insert_avl_node(tree->root, val, &(tree->insert_compare_count), &is_inserted);
    if (is_inserted) tree->node_count++;
}

int search_avl(AVLStructure* tree, int target, int* compare_count) {
    *compare_count = 0;
    AVLNode* curr = tree->root;
    while (curr != NULL) {
        (*compare_count)++;
        if (curr->key == target) {
            return 1;
        }
        else if (target < curr->key) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    return 0;
}

int get_avl_height(const AVLNode* node) {
    if (node == NULL) return 0;
    int left_h = get_avl_height(node->left);
    int right_h = get_avl_height(node->right);
    return (left_h > right_h ? left_h : right_h) + 1;
}

// ==========================================
// 메인 실행
// ==========================================
int main() {
    srand((unsigned int)time(NULL));

    ArrayStructure arr;
    BSTStructure bst;
    AVLStructure avl;

    init_array(&arr);
    init_bst(&bst);
    init_avl(&avl);

    int generated_keys[INSERT_COUNT];

    // 1. 100개의 난수 생성 및 삽입
    printf("===============================================================\n");
    printf("[1] 생성된 100개의 정수 목록\n");
    printf("===============================================================\n");
    for (int i = 0; i < INSERT_COUNT; i++) {
        generated_keys[i] = rand() % (MAX_VAL + 1);
        printf("%4d ", generated_keys[i]);
        if ((i + 1) % 10 == 0) printf("\n");

        insert_array(&arr, generated_keys[i]);
        insert_bst(&bst, generated_keys[i]);
        insert_avl(&avl, generated_keys[i]);
    }
    printf("\n");

    // 2. 50개의 탐색 대상 난수 생성
    int search_targets[SEARCH_COUNT];
    for (int i = 0; i < SEARCH_COUNT; i++) {
        search_targets[i] = rand() % (MAX_VAL + 1);
    }

    printf("===============================================================\n");
    printf("[2] 생성된 50개의 탐색 대상 목록\n");
    printf("===============================================================\n");
    for (int i = 0; i < SEARCH_COUNT; i++) {
        printf("%4d ", search_targets[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");

    // 3. 개별 탐색 결과 (테이블 형식으로 간결화)
    printf("===============================================================\n");
    printf("[3] 개별 탐색 결과 요약 표 (총 50회)\n");
    printf("===============================================================\n");
    printf("%-3s | %-6s | %-7s | %-12s | %-10s | %-10s\n",
        "번호", "탐색 키", "탐색 결과", "순차 비교(회)", "BST 비교(회)", "AVL 비교(회)");
    printf("------+--------+----------+---------------+--------------+--------------\n");

    for (int i = 0; i < SEARCH_COUNT; i++) {
        int target = search_targets[i];
        int seq_comp = 0, bst_comp = 0, avl_comp = 0;

        int seq_found = search_array(&arr, target, &seq_comp);
        int bst_found = search_bst(&bst, target, &bst_comp);
        int avl_found = search_avl(&avl, target, &avl_comp);

        arr.total_search_compare_count += seq_comp;
        bst.total_search_compare_count += bst_comp;
        avl.total_search_compare_count += avl_comp;

        // 1줄로 한눈에 비교 가능하도록 표 형태로 출력
        printf("%-4d | %-6d | %-8s | %-13d | %-12d | %-10d\n",
            i + 1, target, seq_found ? "성공" : "실패", seq_comp, bst_comp, avl_comp);
    }
    printf("===============================================================\n");

    // 4. 프로그램 실행 최종 요약 리포트
    int unique_count = arr.size;
    int duplicate_count = INSERT_COUNT - unique_count;

    printf("\n===============================================================\n");
    printf("[4] 프로그램 실행 최종 요약 리포트\n");
    printf("===============================================================\n");
    printf("저장된 값의 수 : %d\n", unique_count);
    printf("중복된 값의 수 : %d\n\n", duplicate_count);

    printf("자료구조 생성 과정 비교 횟수\n");
    printf("배열 생성 총 비교 횟수     : %lld회\n", arr.insert_compare_count);
    printf("BST 생성 총 비교 횟수      : %lld회\n", bst.insert_compare_count);
    printf("AVL 트리 생성 총 비교 횟수 : %lld회\n\n", avl.insert_compare_count);

    printf("자료구조 완료 후 상태\n");
    printf("배열의 길이   : %d\n", arr.size);
    printf("BST의 높이    : %d\n", get_bst_height(bst.root));
    printf("AVL 트리의 높이: %d\n\n", get_avl_height(avl.root));

    printf("탐색 수행 정보\n");
    printf("총 탐색 횟수 : %d회\n\n", SEARCH_COUNT);

    printf("순차 탐색 결과\n");
    printf("총 비교 횟수  : %lld회\n", arr.total_search_compare_count);
    printf("평균 비교 횟수: %.2f회\n\n", (double)arr.total_search_compare_count / SEARCH_COUNT);

    printf("BST 탐색 결과\n");
    printf("총 비교 횟수  : %lld회\n", bst.total_search_compare_count);
    printf("평균 비교 횟수: %.2f회\n\n", (double)bst.total_search_compare_count / SEARCH_COUNT);

    printf("AVL 트리 탐색 결과\n");
    printf("총 비교 횟수  : %lld회\n", avl.total_search_compare_count);
    printf("평균 비교 횟수: %.2f회\n", (double)avl.total_search_compare_count / SEARCH_COUNT);
    printf("===============================================================\n");

    return 0;
}
