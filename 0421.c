#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int degree;
    float* coef;
} Polynomial;

// 순차 리스트 생성 및 할당
Polynomial create_poly(int degree) {
    Polynomial p;
    p.degree = degree;
    p.coef = (float*)malloc(sizeof(float) * (degree + 1));
    for (int i = 0; i <= degree; i++) p.coef[i] = 0;
    return p;
}

// 사용자로부터 다항식을 형식에 맞춰 입력받는 함수
void input_poly(Polynomial* p) {
    int exp;
    float co;
    printf("형식(계수X^지수 + ...)에 맞춰 입력 (종료는 -1 입력):\n");

    while (1) {
        // "계수X^지수" 형태를 파싱하고 뒤의 "+" 혹은 공백을 처리함
        if (scanf("%fX^%d", &co, &exp) != 2) break;

        if (exp <= p->degree) {
            p->coef[exp] = co;
        }

        // 다음 항을 위한 "+" 기호 소비 (공백 포함 가능)
        char next;
        scanf(" %c", &next);
        if (next != '+') break;
    }
}

// 곱셈 로직 (이전과 동일한 순차 리스트 연산)
Polynomial multiply_poly(Polynomial A, Polynomial B) {
    Polynomial C = create_poly(A.degree + B.degree);
    for (int i = 0; i <= A.degree; i++) {
        for (int j = 0; j <= B.degree; j++) {
            if (A.coef[i] != 0 && B.coef[j] != 0) {
                C.coef[i + j] += A.coef[i] * B.coef[j];
            }
        }
    }
    return C;
}

void print_poly(Polynomial p, char* name) {
    printf("%s: ", name);
    for (int i = p.degree; i >= 0; i--) {
        if (p.coef[i] != 0) {
            printf("%.1fX^%d %s", p.coef[i], i, (i > 0) ? "+ " : "");
        }
    }
    printf("\n");
}

int main() {
    int degA, degB;

    printf("다항식 A의 최고 차수: ");
    scanf("%d", &degA);
    Polynomial A = create_poly(degA);
    input_poly(&A);

    printf("다항식 B의 최고 차수: ");
    scanf("%d", &degB);
    Polynomial B = create_poly(degB);
    input_poly(&B);

    Polynomial C = multiply_poly(A, B);

    print_poly(A, "A(x)");
    print_poly(B, "B(x)");
    print_poly(C, "Result C(x)");

    free(A.coef);
    free(B.coef);
    free(C.coef);
    return 0;
}
