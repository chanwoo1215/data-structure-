#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "my_fraction.h"

int main(int argc, char** argv) {
    
    printf("프로그램 실행 성공  : %d\n", argc);

    if (argc != 3) {
        printf("오류: 분수를 입력해야 함\n");
        return 1;
    }

    Fraction width = fromString(argv[1]);
    Fraction height = fromString(argv[2]);

    Fraction sum = add(width, height);
    Fraction perimeter = add(sum, sum);
    Fraction area = multiply(width, height);

    printf("Perimeter: %d/%d, Area: %d/%d\n",
        perimeter.num, perimeter.den, area.num, area.den);

    return 0;
}

