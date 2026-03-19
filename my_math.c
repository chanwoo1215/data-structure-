#include "my_math.h"
#include <stdlib.h>

int get_gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int get_lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int gcd = get_gcd(a, b);
    return abs(a * b) / gcd;
}
