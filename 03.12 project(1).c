#include <stdio.h>

int main() {
    float input;
    scanf_s("%f", &input);

    unsigned int data = *(unsigned int*)&input;

    for (int i = 31; i >= 0; i--) {
        printf("%d", (data >> i) & 1);

        if (i == 31 || i == 23) {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}
