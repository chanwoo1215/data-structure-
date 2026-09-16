#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXLEN  1024
#define MAXSTK  1024

typedef struct LinkedNode {
    char data;
    struct LinkedNode* left;
    struct LinkedNode* right;
} LinkedNode;

LinkedNode* create_node(char data) {
    LinkedNode* node = (LinkedNode*)malloc(sizeof(LinkedNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

LinkedNode* parse_tree(const char** p) {
    while (**p && isspace((unsigned char)**p)) (*p)++;

    if (**p == '\0' || **p == ')' || **p == ',') {
        return NULL;
    }

    if (!isalnum((unsigned char)**p)) {
        return NULL;
    }

    char data = **p;
    (*p)++;

    LinkedNode* node = create_node(data);

    while (**p && isspace((unsigned char)**p)) (*p)++;

    if (**p == '(') {
        (*p)++;

        while (**p && isspace((unsigned char)**p)) (*p)++;
        if (**p != ',' && **p != ')') {
            node->left = parse_tree(p);
        }

        while (**p && isspace((unsigned char)**p)) (*p)++;
        if (**p == ',') {
            (*p)++;
            while (**p && isspace((unsigned char)**p)) (*p)++;
            if (**p != ')') {
                node->right = parse_tree(p);
            }
        }

        while (**p && isspace((unsigned char)**p)) (*p)++;
        if (**p == ')') {
            (*p)++;
        }
    }

    return node;
}

void print_tree_sideways(LinkedNode* root, int depth) {
    if (root != NULL) {
        print_tree_sideways(root->right, depth + 1);
        for (int i = 0; i < depth; i++) printf("    ");
        if (depth > 0) printf("└── ");
        printf("%c\n", root->data);
        print_tree_sideways(root->left, depth + 1);
    }
}

void get_tree_info(LinkedNode* root, int depth, int* total, int* leaf, int* nonleaf, int* height, int* degree) {
    if (root == NULL) return;

    (*total)++;
    if (depth + 1 > *height) *height = depth + 1;

    int child_cnt = 0;
    if (root->left != NULL) child_cnt++;
    if (root->right != NULL) child_cnt++;

    if (child_cnt > *degree) *degree = child_cnt;
    if (child_cnt == 0) (*leaf)++;
    else (*nonleaf)++;

    get_tree_info(root->left, depth + 1, total, leaf, nonleaf, height, degree);
    get_tree_info(root->right, depth + 1, total, leaf, nonleaf, height, degree);
}

int is_full_binary_tree(LinkedNode* root, int* height) {
    if (root == NULL) {
        *height = 0;
        return 1;
    }
    int left_h = 0, right_h = 0;
    if (!is_full_binary_tree(root->left, &left_h)) return 0;
    if (!is_full_binary_tree(root->right, &right_h)) return 0;

    if (left_h != right_h) return 0;
    *height = left_h + 1;
    return 1;
}

int is_complete_binary_tree(LinkedNode* root) {
    if (root == NULL) return 1;

    LinkedNode* queue[MAXSTK];
    int front = 0, rear = 0;
    queue[rear++] = root;

    int flag = 0;

    while (front < rear) {
        LinkedNode* curr = queue[front++];

        if (curr == NULL) {
            flag = 1;
        }
        else {
            if (flag) return 0;
            queue[rear++] = curr->left;
            queue[rear++] = curr->right;
        }
    }
    return 1;
}

int is_skewed_binary_tree(LinkedNode* root) {
    if (root == NULL) return 1;

    if (root->left != NULL && root->right != NULL) return 0;

    return is_skewed_binary_tree(root->left) && is_skewed_binary_tree(root->right);
}

void free_tree(LinkedNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main(void) {
    char line[MAXLEN];

    printf("괄호 표기법을 입력하세요 (예: A(B(D,E),C(,F)) 또는 A(,B)): ");
    if (!fgets(line, sizeof(line), stdin)) {
        printf("입력 오류\n");
        return 1;
    }
    line[strcspn(line, "\n")] = '\0';

    const char* p = line;
    LinkedNode* root = parse_tree(&p);

    if (root == NULL) {
        printf("오류: 올바른 트리가 생성되지 않았습니다.\n");
        return 1;
    }

    int total = 0, leaf = 0, nonleaf = 0, height = 0, degree = 0;
    get_tree_info(root, 0, &total, &leaf, &nonleaf, &height, &degree);

    printf("\n[이진트리 구조]\n");
    print_tree_sideways(root, 0);

    printf("\n[트리 정보]\n");
    printf("전체 노드의 수   : %d\n", total);
    printf("단말 노드의 수   : %d\n", leaf);
    printf("비단말 노드의 수 : %d\n", nonleaf);
    printf("트리의 높이      : %d\n", height);
    printf("트리의 차수      : %d\n", degree);

    int dummy_h = 0;
    int is_full = is_full_binary_tree(root, &dummy_h);
    int is_complete = is_complete_binary_tree(root);
    int is_skewed = is_skewed_binary_tree(root);

    printf("\n[이진트리 유형 판별]\n");
    printf("포화 이진트리 : %s\n", is_full ? "예 " : "아니오 ");
    printf("완전 이진트리 : %s\n", is_complete ? "예 " : "아니오 ");
    printf("편향 이진트리 : %s\n", is_skewed ? "예 " : "아니오 ");

    free_tree(root);

    return 0;
}
