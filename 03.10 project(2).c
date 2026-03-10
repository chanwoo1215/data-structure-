#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    float input;

    printf("입력값: ");
    if (scanf("%f", &input) != 1) return 1;

    int16_t fixed_point = (int16_t)(input * 256);

    printf("출력값: ");
    for (int i = 15; i >= 0; i--) {
        int bit = (fixed_point >> i) & 1;
        printf("%d", bit);
        if (i == 8) {
            printf(".");
        }
    }
    printf("\n");

    system("pause");
    return 0;
}
