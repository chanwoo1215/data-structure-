#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXLEN  1024
#define MAXSTK  1024

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

    if (next_nonspace(s, 0) == '\0') { strcpy(msg, "입력이 비어 있습니다."); return 0; }

    for (i = 0; s[i]; i++) {
        ch = s[i];
        if (isspace((unsigned char)ch)) continue;

        if (isalnum((unsigned char)ch)) {
            if (state != 0) { strcpy(msg, "노드 이름이 올 수 없는 위치입니다."); return 0; }
            if (depth == 0) {
                if (seen_root) { strcpy(msg, "루트 노드가 2개 이상입니다."); return 0; }
                seen_root = 1;
            }
            state = 1;
        }
        else if (ch == '(') {
            if (state != 1) { strcpy(msg, "'(' 앞에는 노드 이름이 와야 합니다."); return 0; }
            if (next_nonspace(s, i + 1) == ')') { strcpy(msg, "'()' 처럼 빈 자식 목록은 올 수 없습니다."); return 0; }
            depth++; state = 0;
        }
        else if (ch == ')') {
            if (state != 1) { strcpy(msg, "')' 앞에는 노드나 부분트리가 와야 합니다."); return 0; }
            depth--;
            if (depth < 0) { strcpy(msg, "닫는 괄호가 여는 괄호보다 많습니다."); return 0; }
            state = 1;
        }
        else if (ch == ',') {
            if (state != 1) { strcpy(msg, "',' 위치가 올바르지 않습니다."); return 0; }
            if (depth == 0) { strcpy(msg, "괄호 밖에서는 ','를 쓸 수 없습니다."); return 0; }
            state = 0;
        }
        else {
            sprintf(msg, "허용되지 않는 문자 '%c' 가 있습니다.", ch);
            return 0;
        }
    }
    if (depth != 0) { strcpy(msg, "괄호의 짝이 맞지 않습니다."); return 0; }
    if (state != 1) { strcpy(msg, "표기가 완성되지 않았습니다."); return 0; }
    return 1;
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

    i_init(&cnt);
    c_init(&nodes);

    printf("트리의 괄호 표기법을 입력하세요 (예: A(B(E,F),C,D(G))): ");
    if (!fgets(line, sizeof(line), stdin)) {
        printf("오류: 입력을 읽을 수 없습니다.\n");
        return 1;
    }
    line[strcspn(line, "\n")] = '\0';

    if (!validate(line, msg)) {
        printf("오류: 올바른 트리의 괄호 표기법이 아닙니다. (%s)\n", msg);
        return 1;
    }

    printf("\n[트리 구조]\n");

    for (i = 0; line[i]; i++) {
        char ch = line[i];
        if (isspace((unsigned char)ch)) continue;

        if (isalnum((unsigned char)ch)) {
            char nx = next_nonspace(line, i + 1);
            int  j;

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

            for (j = 0; j < d; j++) printf("    ");
            if (d > 0) printf("+---");
            printf("%c\n", ch);
        }
        else if (ch == '(') {
            int k;
            for (k = i - 1; k >= 0 && isspace((unsigned char)line[k]); k--);
            c_push(&nodes, line[k]);
            i_push(&cnt, 0);
            d++;
        }
        else if (ch == ')') {
            int c = i_pop(&cnt);
            if (c > degree) degree = c;
            c_pop(&nodes);
            d--;
            if (collecting && d < target_depth) collecting = 0;
        }
    }

    printf("\n[트리 정보]\n");
    printf("전체 노드의 수   : %d\n", total);
    printf("단말 노드의 수   : %d\n", leaf);
    printf("비단말 노드의 수 : %d\n", nonleaf);
    printf("트리의 높이      : %d\n", height);
    printf("트리의 차수      : %d\n", degree);

    if (!found_target) {
        printf("노드 %c 의 부모  : (노드 %c 가 트리에 없습니다)\n", target, target);
        printf("노드 %c 의 자식  : (노드 %c 가 트리에 없습니다)\n", target, target);
    }
    else {
        if (parent_of_target == '\0')
            printf("노드 %c 의 부모  : 없음 (루트)\n", target);
        else
            printf("노드 %c 의 부모  : %c\n", target, parent_of_target);

        printf("노드 %c 의 자식  : ", target);
        if (nchild == 0) printf("없음 (단말 노드)");
        else for (i = 0; i < nchild; i++) printf("%c%s", children[i], (i == nchild - 1) ? "" : ", ");
        printf("\n");
    }
    return 0;
}
