#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "my_fraction.h"

int main() {
    char s1[20], op_str[10], s2[20];
    Fraction f1, f2, result;

    printf("분수 연산을 입력 : ");

    
    if (scanf("%s %s %s", s1, op_str, s2) != 3) {
        printf("입력 형식의 오류 발생.\n");
        system("pause");
        return 1;
    }

    f1 = fromString(s1);
    f2 = fromString(s2);
    char op = op_str[0];

    if (op == '+') result = add(f1, f2);
    else if (op == '-') result = subtract(f1, f2);
    else if (op == '*') result = multiply(f1, f2);
    else if (op == '/') result = divide(f1, f2);
    else {
        printf("지원하지 않는 연산자.\n");
        system("pause");
        return 1;
    }

    printf("결과: ");
    printFraction(result);

    system("pause");
    return 0;
}
