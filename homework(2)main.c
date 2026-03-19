#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "my_math.h"

int main() {
    int n1, n2;

    
    if (scanf("%d %d", &n1, &n2) != 2) {
        return 1;
    }

    int gcd = get_gcd(n1, n2);
    int lcm = get_lcm(n1, n2);

 
    printf("GCD: %d, LCM: %d\n", gcd, lcm);

    return 0;
}
