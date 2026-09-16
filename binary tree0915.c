#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#define MAXLEN  1024
#define MAXSTK  1024
#define MAX_NODES 100

// 배열을 이용한 이진트리 노드 구조체 (인덱스 기반 링크)
typedef struct {
    char data;
    int left;   // 왼쪽 자식의 배열 인덱스 (없으면 -1)
    int right;  // 오른쪽 자식의 배열 인덱스 (없으면 -1)
} ArrayNode;

// 전역 배열 트리 선언
ArrayNode tree_array[MAX_NODES];
int node_count = 0;

typedef struct { int  data[MAXSTK]; int top; } IntStack;
typedef struct { char data[MAXSTK]; int top; } ChrStack;

static void i_init(IntStack* s) { s->top = -1; }
static void i_push(IntStack* s, int v) { if (s->top < MAXSTK - 1) s->data[++s->top] = v; }
static int  i_pop(IntStack* s) { return (s->top < 0) ? -1 : s->data[s->top--]; }
static int  i_empty(IntStack* s) { return s->top < 0; }

static void c_init(ChrStack* s) { s->top = -1; }
static void c_push(ChrStack* s, char v) { if (s->top < MAXSTK - 1) s->data[++s->top] = v; }
static char c_pop(ChrStack* s) { return (s->top < 0) ? '\0' : s->data[s->top--]; }
static char c_peek(ChrStack* s) { return (s->top < 0) ? '\0' : s->data[s->top]; }

static char next_nonspace(const char* s, int i)
{
    int j;
    for (j = i; s[j]; j++)
        if (!isspace((unsigned char)s[j])) return s[j];
    return '\0';
}

static int validate(const char* s, char* msg)
{
    int i, depth = 0, state = 0, seen_root = 0;
    char ch;

    if (next_nonspace(s, 0) == '\0') { strcpy(msg, "입력이 비었음."); return 0; }

    for (i = 0; s[i]; i++) {
        ch = s[i];
        if (isspace((unsigned char)ch)) continue;

        if (isalnum((unsigned char)ch)) {
            if (state != 0) { strcpy(msg, "노드 이름이 올 수 없는 위치."); return 0; }
            if (depth == 0) {
                if (seen_root) { strcpy(msg, "루트 노드가 2개 이상"); return 0; }
                seen_root = 1;
            }
            state = 1;
        }
        else if (ch == '(') {
            if (state != 1) { strcpy(msg, "'(' 앞에는 노드 이름이 와야함."); return 0; }
            if (next_nonspace(s, i + 1) == ')') { strcpy(msg, "'()' 처럼 빈 자식 목록은 올 수 없음."); return 0; }
            depth++; state = 0;
        }
        else if (ch == ')') {
            if (state != 1) { strcpy(msg, "')' 앞에는 노드나 부분트리가 와야함."); return 0; }
            depth--;
            if (depth < 0) { strcpy(msg, "닫는 괄호가 여는 괄호보다 많음."); return 0; }
            state = 1;
        }
        else if (ch == ',') {
            if (state != 1) { strcpy(msg, "',' 위치가 올바르지 않음."); return 0; }
            if (depth == 0) { strcpy(msg, "괄호 밖에서는 ','를 쓸 수 없음."); return 0; }
            state = 0;
        }
        else {
            sprintf(msg, "허용되지 않는 문자 '%c' 가 있음.", ch);
            return 0;
        }
    }
    if (depth != 0) { strcpy(msg, "괄호의 짝이 맞지 않음."); return 0; }
    if (state != 1) { strcpy(msg, "표기가 완성되지 않았음."); return 0; }
    return 1;
}


void print_tree_sideways(int idx, int depth) {
    if (idx != -1) {
        print_tree_sideways(tree_array[idx].right, depth + 1);
        for (int i = 0; i < depth; i++) printf("    ");
        if (depth > 0) printf("└── ");
        printf("%c\n", tree_array[idx].data);
        print_tree_sideways(tree_array[idx].left, depth + 1);
    }
}
int is_full_binary_tree(int idx, int* height) {
    if (idx == -1) {
        *height = 0;
        return 1;
    }
    int left_height = 0, right_height = 0;
    if (!is_full_binary_tree(tree_array[idx].left, &left_height)) return 0;
    if (!is_full_binary_tree(tree_array[idx].right, &right_height)) return 0;

    if (left_height != right_height) return 0;
    *height = left_height + 1;
    return 1;
}


int is_complete_binary_tree(int root_idx) {
    if (root_idx == -1) return 1;

    int queue[MAX_NODES];
    int front = 0, rear = 0;
    queue[rear++] = root_idx;

    int flag = 0; 

    while (front < rear) {
        int curr = queue[front++];

        if (curr == -1) {
            flag = 1;
        }
        else {
            if (flag) return 0; 
            queue[rear++] = tree_array[curr].left;
            queue[rear++] = tree_array[curr].right;
        }
    }
    return 1;
}


int is_skewed_binary_tree(int idx) {
    if (idx == -1) return 1;

    int left = tree_array[idx].left;
    int right = tree_array[idx].right;

    
    if (left != -1 && right != -1) return 0;

    return is_skewed_binary_tree(left) && is_skewed_binary_tree(right);
}

int main(void)
{
    char  line[MAXLEN], msg[256];
    int   i, d = 0;
    int   total = 0, leaf = 0, nonleaf = 0, height = 0, degree = 0;
    char  target = 'C';
    char  parent_of_target = '\0';
    int   found_target = 0;
    char  children[MAXSTK]; int nchild = 0;
    int   collecting = 0, target_depth = -1;

    IntStack cnt;
    ChrStack nodes;

    typedef struct { int data[MAXSTK]; int top; } IndexStack;
    IndexStack idx_stack;
    idx_stack.top = -1;

    typedef struct { int data[MAXSTK]; int top; } FlagStack;
    FlagStack flag_stack;
    flag_stack.top = -1;

    i_init(&cnt);
    c_init(&nodes);

    printf("트리의 괄호 표기법을 입력 (ex): A(B(D,E),C(F,G))): ");
    if (!fgets(line, sizeof(line), stdin)) {
        printf("오류: 입력을 읽을 수 없음.\n");
        return 1;
    }
    line[strcspn(line, "\n")] = '\0';

    if (!validate(line, msg)) {
        printf("오류: 올바른 트리의 괄호 표기법이 아님. (%s)\n", msg);
        return 1;
    }

    for (int k = 0; k < MAX_NODES; k++) {
        tree_array[k].data = '\0';
        tree_array[k].left = -1;
        tree_array[k].right = -1;
    }

    int curr_idx = -1;
    int child_flag = 0;

    for (i = 0; line[i]; i++) {
        char ch = line[i];
        if (isspace((unsigned char)ch)) continue;

        if (isalnum((unsigned char)ch)) {
            char nx = next_nonspace(line, i + 1);

            total++;
            if (d + 1 > height) height = d + 1;
            if (nx == '(') nonleaf++; else leaf++;

            if (!i_empty(&cnt)) cnt.data[cnt.top]++;

            if (ch == target && !found_target) {
                found_target = 1;
                parent_of_target = c_peek(&nodes);
                if (nx == '(') { collecting = 1; target_depth = d + 1; }
            }
            else if (collecting && d == target_depth) {
                children[nchild++] = ch;
            }

            int new_idx = node_count++;
            tree_array[new_idx].data = ch;
            tree_array[new_idx].left = -1;
            tree_array[new_idx].right = -1;

            if (curr_idx != -1) {
                if (child_flag == 0) {
                    tree_array[curr_idx].left = new_idx;
                }
                else {
                    tree_array[curr_idx].right = new_idx;
                }
            }
            curr_idx = new_idx;
        }
        else if (ch == '(') {
            int k;
            for (k = i - 1; k >= 0 && isspace((unsigned char)line[k]); k--);
            c_push(&nodes, line[k]);
            i_push(&cnt, 0);
            d++;

            idx_stack.data[++idx_stack.top] = curr_idx;
            flag_stack.data[++flag_stack.top] = child_flag;
            child_flag = 0;
        }
        else if (ch == ')') {
            int c = i_pop(&cnt);
            if (c > degree) degree = c;
            c_pop(&nodes);
            d--;
            if (collecting && d < target_depth) collecting = 0;

            if (idx_stack.top >= 0) {
                curr_idx = idx_stack.data[idx_stack.top--];
                child_flag = flag_stack.data[flag_stack.top--];
            }
        }
        else if (ch == ',') {
            if (idx_stack.top >= 0) {
                curr_idx = idx_stack.data[idx_stack.top];
            }
            child_flag = 1;
        }
    }

    printf("\n[트리 구조]\n");
    print_tree_sideways(0, 0);

    printf("\n[트리 정보]\n");
    printf("전체 노드의 수   : %d\n", total);
    printf("단말 노드의 수   : %d\n", leaf);
    printf("비단말 노드의 수 : %d\n", nonleaf);
    printf("트리의 높이      : %d\n", height);
    printf("트리의 차수      : %d\n", degree);

    
    int dummy_h = 0;
    int is_full = is_full_binary_tree(0, &dummy_h);
    int is_complete = is_complete_binary_tree(0);
    int is_skewed = is_skewed_binary_tree(0);

    printf("\n[이진트리 유형 판별]\n");
    printf("포화 이진트리인가?   : %s\n", is_full ? "맞음" : "아님");
    printf("완전 이진트리인가: %s\n", is_complete ? "맞음" : "아님");
    printf("편향 이진트리인가?  : %s\n", is_skewed ? "맞음" : "아님");

    return 0;
}
