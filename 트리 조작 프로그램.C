#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif


#define MAX_PATH_LEN 256
#define MAX_TOKENS 100

// --- 1. 노드 및 이진트리 Structure 정의 ---
typedef struct TreeNode {
    char data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct BinaryTree {
    TreeNode* root;
    int max_size;
} BinaryTree;

// --- 2. 이진트리 ADT 연산 구현 ---

// 새 노드 생성
TreeNode* create_node(char data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// create_btree(size): 빈 이진트리 생성
BinaryTree* create_btree(int size) {
    BinaryTree* tree = (BinaryTree*)malloc(sizeof(BinaryTree));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->max_size = size;
    return tree;
}

// insert_root(tree, value): 루트 노드 생성
BinaryTree* insert_root(BinaryTree* tree, char value) {
    if (tree == NULL) return NULL;
    if (tree->root != NULL) return NULL; // 이미 루트가 존재하는 경우
    tree->root = create_node(value);
    return tree;
}

// 경로 탐색 헬퍼 함수 (/A/B/C 형식)
TreeNode* find_node_by_path(TreeNode* root, const char* path) {
    if (root == NULL || path == NULL || strlen(path) == 0) return NULL;

    char temp[MAX_PATH_LEN];
    strncpy(temp, path, MAX_PATH_LEN);
    temp[MAX_PATH_LEN - 1] = '\0';

    // 토큰 분리
    char* tokens[MAX_TOKENS];
    int token_count = 0;
    char* token = strtok(temp, "/");
    while (token != NULL) {
        tokens[token_count++] = token;
        token = strtok(NULL, "/");
    }

    if (token_count == 0) return NULL;

    // 루트 데이터 검증
    if (root->data != tokens[0][0] || strlen(tokens[0]) != 1) {
        return NULL; // 경로 시작점 불일치
    }

    TreeNode* curr = root;
    for (int i = 1; i < token_count; i++) {
        char target_data = tokens[i][0];
        if (strlen(tokens[i]) != 1) return NULL;

        if (curr->left && curr->left->data == target_data) {
            curr = curr->left;
        }
        else if (curr->right && curr->right->data == target_data) {
            curr = curr->right;
        }
        else {
            return NULL; // 경로 상 노드 미존재
        }
    }
    return curr;
}

// insert_child(tree, parent, child, value)
BinaryTree* insert_child(BinaryTree* tree, TreeNode* parent, char child, char value) {
    if (tree == NULL || parent == NULL) return NULL;

    // 조건: 동일 부모의 양쪽 자식이 같은 데이터를 가질 수 없음
    if ((parent->left && parent->left->data == value) ||
        (parent->right && parent->right->data == value)) {
        return NULL;
    }

    if (child == 'L' || child == 'l') {
        if (parent->left != NULL) return NULL; // 위치에 이미 존재
        parent->left = create_node(value);
    }
    else if (child == 'R' || child == 'r') {
        if (parent->right != NULL) return NULL; // 위치에 이미 존재
        parent->right = create_node(value);
    }
    else {
        return NULL;
    }
    return tree;
}

// delete_node(tree, leaf): 단말 노드 삭제
BinaryTree* delete_node(BinaryTree* tree, TreeNode* leaf) {
    if (tree == NULL || tree->root == NULL || leaf == NULL) return NULL;

    // 단말 노드 검증
    if (leaf->left != NULL || leaf->right != NULL) return NULL;

    if (tree->root == leaf) { // 루트 노드 삭제
        free(tree->root);
        tree->root = NULL;
        return tree;
    }

    // 부모 노드 찾기
    // 부모 탐색용 큐
    TreeNode* queue[1000];
    int front = 0, rear = 0;
    queue[rear++] = tree->root;

    TreeNode* parent = NULL;
    while (front < rear) {
        TreeNode* curr = queue[front++];
        if (curr->left == leaf || curr->right == leaf) {
            parent = curr;
            break;
        }
        if (curr->left) queue[rear++] = curr->left;
        if (curr->right) queue[rear++] = curr->right;
    }

    if (parent != NULL) {
        if (parent->left == leaf) parent->left = NULL;
        else if (parent->right == leaf) parent->right = NULL;
        free(leaf);
    }
    return tree;
}

// update_value(tree, node, value): 노드 데이터 변경
BinaryTree* update_value(BinaryTree* tree, TreeNode* node, char value) {
    if (tree == NULL || node == NULL) return NULL;

    // 부모 찾기
    TreeNode* parent = NULL;
    if (tree->root != node) {
        TreeNode* queue[1000];
        int front = 0, rear = 0;
        queue[rear++] = tree->root;
        while (front < rear) {
            TreeNode* curr = queue[front++];
            if (curr->left == node || curr->right == node) {
                parent = curr;
                break;
            }
            if (curr->left) queue[rear++] = curr->left;
            if (curr->right) queue[rear++] = curr->right;
        }
    }

    // 변경 후 형제 노드와 중복 데이터 발생하는지 확인
    if (parent != NULL) {
        TreeNode* sibling = (parent->left == node) ? parent->right : parent->left;
        if (sibling != NULL && sibling->data == value) {
            return NULL; // 중복 제약 조건 위반
        }
    }

    node->data = value;
    return tree;
}

// read_child(tree, parent): 자식 정보 출력
void read_child(BinaryTree* tree, TreeNode* parent) {
    if (tree == NULL || parent == NULL) return;

    if (parent->left == NULL && parent->right == NULL) {
        printf("자식 노드가 존재하지 않습니다.\n");
        return;
    }

    if (parent->left != NULL) {
        printf("L: %c\n", parent->left->data);
    }
    if (parent->right != NULL) {
        printf("R: %c\n", parent->right->data);
    }
}

// print_btree 재귀 출력 헬퍼 (오른쪽-루트-왼쪽 중위 순회 형태)
void print_helper(TreeNode* node, int level) {
    if (node == NULL) return;

    print_helper(node->right, level + 1);

    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    if (level > 0) {
        printf("+--");
    }
    printf("%c\n", node->data);

    print_helper(node->left, level + 1);
}

// print_btree(tree): 트리를 왼쪽으로 눕힌 형태로 출력
void print_btree(BinaryTree* tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("트리가 비어 있습니다.\n");
        return;
    }
    print_helper(tree->root, 0);
}

// destroy_btree_helper 메모리 해제
void destroy_helper(TreeNode* node) {
    if (node == NULL) return;
    destroy_helper(node->left);
    destroy_helper(node->right);
    free(node);
}

// destroy_btree(tree): 메모리 해제
void destroy_btree(BinaryTree* tree) {
    if (tree == NULL) return;
    destroy_helper(tree->root);
    free(tree);
}

// --- 3. 사용자 명령어 처리 루틴 ---

void process_command(BinaryTree* tree) {
    char line[512];
    while (1) {
        printf("\nCommand > ");
        if (!fgets(line, sizeof(line), stdin)) break;

        // 개행 제거
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        // 공백 기반 명령 파싱
        char* args[10];
        int arg_count = 0;
        char* ptr = strtok(line, " ");
        while (ptr != NULL && arg_count < 10) {
            args[arg_count++] = ptr;
            ptr = strtok(NULL, " ");
        }

        if (arg_count == 0) continue;

        char* cmd = args[0];

        // 1) Insert / I
        if (strcasecmp(cmd, "Insert") == 0 || strcasecmp(cmd, "I") == 0) {
            if (arg_count == 2) {
                // Insert /A (루트 생성)
                char* path = args[1];
                if (path[0] != '/' || strlen(path) != 2 || !isupper(path[1])) {
                    printf("오류: 올바르지 않은 루트 생성 형식입니다.\n");
                    continue;
                }
                if (tree->root != NULL) {
                    printf("오류: 이미 루트 노드가 존재합니다.\n");
                    continue;
                }
                insert_root(tree, path[1]);
                printf("루트 노드 '%c' 생성 완료.\n", path[1]);
            }
            else if (arg_count == 4) {
                // Insert parent-node child new-data
                char* path = args[1];
                char child = toupper(args[2][0]);
                char new_data = toupper(args[3][0]);

                if (tree->root == NULL) {
                    printf("오류: 트리가 비어 있습니다.\n");
                    continue;
                }
                if (child != 'L' && child != 'R') {
                    printf("오류: child 위치는 L 또는 R 이어야 합니다.\n");
                    continue;
                }
                if (!isupper(new_data) || strlen(args[3]) != 1) {
                    printf("오류: 데이터는 영문 대문자 한 글자여야 합니다.\n");
                    continue;
                }

                TreeNode* parent = find_node_by_path(tree->root, path);
                if (parent == NULL) {
                    printf("오류: 지정한 부모 노드 경로가 존재하지 않습니다.\n");
                    continue;
                }

                // 자식이 2개 미만인지 확인
                if (parent->left != NULL && parent->right != NULL) {
                    printf("오류: 해당 부모 노드는 이미 자식이 2개 존재합니다.\n");
                    continue;
                }

                if (insert_child(tree, parent, child, new_data) == NULL) {
                    printf("오류: 자식 추가에 실패했습니다 (동일 부모 아래 중복 데이터 혹은 자식 위치 이미 존재).\n");
                }
                else {
                    printf("노드 '%c' 추가 완료.\n", new_data);
                }
            }
            else {
                printf("오류: Insert 명령의 인자 개수가 올바르지 않습니다.\n");
            }
        }
        // 2) Delete / D
        else if (strcasecmp(cmd, "Delete") == 0 || strcasecmp(cmd, "D") == 0) {
            if (arg_count != 2) {
                printf("오류: Delete 명령의 인자 개수가 올바르지 않습니다.\n");
                continue;
            }
            TreeNode* target = find_node_by_path(tree->root, args[1]);
            if (target == NULL) {
                printf("오류: 삭제할 노드의 경로가 존재하지 않습니다.\n");
                continue;
            }
            if (target->left != NULL || target->right != NULL) {
                printf("오류: 삭제 대상 노드가 단말 노드가 아닙니다.\n");
                continue;
            }
            delete_node(tree, target);
            printf("노드 삭제 완료.\n");
        }
        // 3) Update / U
        else if (strcasecmp(cmd, "Update") == 0 || strcasecmp(cmd, "U") == 0) {
            if (arg_count != 3) {
                printf("오류: Update 명령의 인자 개수가 올바르지 않습니다.\n");
                continue;
            }
            char new_data = toupper(args[2][0]);
            if (!isupper(new_data) || strlen(args[2]) != 1) {
                printf("오류: 데이터는 영문 대문자 한 글자여야 합니다.\n");
                continue;
            }
            TreeNode* target = find_node_by_path(tree->root, args[1]);
            if (target == NULL) {
                printf("오류: 변경할 노드의 경로가 존재하지 않습니다.\n");
                continue;
            }
            if (update_value(tree, target, new_data) == NULL) {
                printf("오류: 변경 시 동일 부모 아래 동일한 데이터를 가진 자식이 존재하게 됩니다.\n");
            }
            else {
                printf("노드 데이터 변경 완료 ('%c').\n", new_data);
            }
        }
        // 4) Read / R
        else if (strcasecmp(cmd, "Read") == 0 || strcasecmp(cmd, "R") == 0) {
            if (arg_count != 2) {
                printf("오류: Read 명령의 인자 개수가 올바르지 않습니다.\n");
                continue;
            }
            TreeNode* target = find_node_by_path(tree->root, args[1]);
            if (target == NULL) {
                printf("오류: 확인할 노드의 경로가 존재하지 않습니다.\n");
                continue;
            }
            read_child(tree, target);
        }
        // 5) Print / P
        else if (strcasecmp(cmd, "Print") == 0 || strcasecmp(cmd, "P") == 0) {
            print_btree(tree);
        }
        // Exit (프로그램 종료 명령어)
        else if (strcasecmp(cmd, "Exit") == 0) {
            break;
        }
        else {
            printf("오류: 알 수 없는 명령어입니다.\n");
        }
    }
}

int main() {
    BinaryTree* tree = create_btree(100);

    printf("=========================================\n");
    printf("     이진트리 관리 프로그램 (ADT 기준)   \n");
    printf("=========================================\n");

    process_command(tree);

    destroy_btree(tree);
    return 0;
}
