#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

// 1. 이진트리 노드 구조체 정의
typedef struct TreeNode {
    char data;                  // 노드의 데이터
    struct TreeNode* left;      // 왼쪽 자식을 가리키는 포인터
    struct TreeNode* right;     // 오른쪽 자식을 가리키는 포인터
} TreeNode;

// 2. 트리 노드를 위한 스택 정의 (구축 및 반복 순회에 사용)
typedef struct NodeStack {
    TreeNode* items[MAX_SIZE];
    int top;
} NodeStack;

void initStack(NodeStack* s) {
    s->top = -1;
}

int isEmpty(NodeStack* s) {
    return s->top == -1;
}

int isFull(NodeStack* s) {
    return s->top == MAX_SIZE - 1;
}

void push(NodeStack* s, TreeNode* node) {
    if (!isFull(s)) {
        s->items[++(s->top)] = node;
    }
}

TreeNode* pop(NodeStack* s) {
    if (!isEmpty(s)) {
        return s->items[(s->top)--];
    }
    return NULL;
}

TreeNode* peek(NodeStack* s) {
    if (!isEmpty(s)) {
        return s->items[s->top];
    }
    return NULL;
}

// 새로운 트리 노드 생성
TreeNode* createNode(char data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 3. 괄호 문자열 파싱 및 트리 생성 (문법 오류 검사 포함)
TreeNode* buildTreeFromNestedPairs(const char* str) {
    NodeStack stack;
    initStack(&stack);

    TreeNode* root = NULL;
    TreeNode* currentNode = NULL;
    int k = 0;
    int length = strlen(str);
    int childCountStack[MAX_SIZE]; // 각 부모 노드의 자식 개수 추적
    int childTop = -1;

    for (int i = 0; i < length; i++) {
        char ch = str[i];

        if (isspace(ch)) continue; // 공백 무시

        if (isalnum(ch)) { // 알파벳/숫자인 경우 (노드 데이터)
            currentNode = createNode(ch);
            if (root == NULL) {
                root = currentNode;
            }
            else {
                TreeNode* parent = peek(&stack);
                if (parent == NULL) {
                    printf("[오류] 트리의 루트가 둘 이상이거나 구조가 올바르지 않습니다.\n");
                    return NULL;
                }

                // 현재 부모의 자식 위치 판단
                if (childCountStack[childTop] == 0) {
                    parent->left = currentNode;
                }
                else if (childCountStack[childTop] == 1) {
                    parent->right = currentNode;
                }
                else {
                    printf("[오류] 한 노드('%c')에 3개 이상의 자식이 존재할 수 없습니다.\n", parent->data);
                    return NULL;
                }
            }
        }
        else if (ch == '(') {
            if (currentNode == NULL) {
                printf("[오류] '(' 앞에 노드 이름이 없습니다.\n");
                return NULL;
            }
            push(&stack, currentNode);
            childCountStack[++childTop] = 0; // 자식 수 0으로 초기화
            currentNode = NULL;
        }
        else if (ch == ',') {
            if (childTop < 0) {
                printf("[오류] 잘못된 쉼표(',') 위치입니다.\n");
                return NULL;
            }
            childCountStack[childTop]++; // 다음 자식(오쪽)으로 이동
            currentNode = NULL;
        }
        else if (ch == ')') {
            if (isEmpty(&stack)) {
                printf("[오류] 괄호(')')의 짝이 맞지 않습니다.\n");
                return NULL;
            }
            pop(&stack);
            childTop--;
            currentNode = NULL;
        }
        else {
            printf("[오류] 허용되지 않는 문자('%c')가 포함되어 있습니다.\n", ch);
            return NULL;
        }
    }

    if (!isEmpty(&stack)) {
        printf("[오류] 닫히지 않은 괄호('(')가 존재합니다.\n");
        return NULL;
    }

    return root;
}

// 4. 반복적 전위 순회 (Iterative Pre-order: V -> L -> R)
void iterativePreorder(TreeNode* root) {
    if (root == NULL) return;

    NodeStack stack;
    initStack(&stack);
    push(&stack, root);

    printf("전위 순회 (Pre-order) : ");
    while (!isEmpty(&stack)) {
        TreeNode* current = pop(&stack);
        printf("%c ", current->data);

        // 스택은 LIFO이므로 오른쪽 자식을 먼저 넣어야 왼쪽 자식이 먼저 처리됨
        if (current->right != NULL) {
            push(&stack, current->right);
        }
        if (current->left != NULL) {
            push(&stack, current->left);
        }
    }
    printf("\n");
}

// 5. 반복적 중위 순회 (Iterative In-order: L -> V -> R)
void iterativeInorder(TreeNode* root) {
    if (root == NULL) return;

    NodeStack stack;
    initStack(&stack);
    TreeNode* current = root;

    printf("중위 순회 (In-order)  : ");
    while (current != NULL || !isEmpty(&stack)) {
        // 왼쪽 자식 노드들을 끝까지 스택에 쌓음
        while (current != NULL) {
            push(&stack, current);
            current = current->left;
        }

        // 더 이상 왼쪽 자식이 없으면 pop하여 출력 후 오른쪽으로 이동
        current = pop(&stack);
        printf("%c ", current->data);
        current = current->right;
    }
    printf("\n");
}

// 6. 반복적 후위 순회 (Iterative Post-order: L -> R -> V)
// 두 개의 스택을 사용하여 역순(V -> R -> L)으로 쌓은 뒤 반전시키는 방식
void iterativePostorder(TreeNode* root) {
    if (root == NULL) return;

    NodeStack s1, s2;
    initStack(&s1);
    initStack(&s2);

    push(&s1, root);

    while (!isEmpty(&s1)) {
        TreeNode* current = pop(&s1);
        push(&s2, current);

        // 왼쪽을 먼저 넣어서 s2에 들어갈 때 오른쪽이 위로 오게 함
        if (current->left != NULL) {
            push(&s1, current->left);
        }
        if (current->right != NULL) {
            push(&s1, current->right);
        }
    }

    printf("후위 순회 (Post-order): ");
    while (!isEmpty(&s2)) {
        TreeNode* current = pop(&s2);
        printf("%c ", current->data);
    }
    printf("\n");
}

// 트리의 모든 노드 동적 할당 해제
void freeTree(TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    char input[MAX_SIZE];

    printf("괄호 표현 이진트리를 입력하세요 (예: A(B(D,E),C)) : ");
    if (scanf("%99s", input) != 1) return 0;

    TreeNode* root = buildTreeFromNestedPairs(input);

    if (root != NULL) {
        printf("\n--- 반복적 순회 결과 ---\n");
        iterativePreorder(root);
        iterativeInorder(root);
        iterativePostorder(root);

        freeTree(root);
    }
    else {
        printf("\n올바르지 않은 괄호 표현입니다. 프로그램을 종료합니다.\n");
    }

    return 0;
}
