#define _CRT_SECURE_NO_WARNINGS
#include "my_fraction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_gcd(int a, int b) {
    a = abs(a); b = abs(b);
    while (b) {
        int temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

Fraction simplify(Fraction f) {
    if (f.den == 0) exit(1);
    int gcd = get_gcd(f.num, f.den);
    f.num /= gcd;
    f.den /= gcd;
    if (f.den < 0) {
        f.num *= -1;
        f.den *= -1;
    }
    return f;
}

Fraction add(Fraction f1, Fraction f2) {
    Fraction res;
    res.num = f1.num * f2.den + f2.num * f1.den;
    res.den = f1.den * f2.den;
    return simplify(res);
}

Fraction subtract(Fraction f1, Fraction f2) {
    Fraction res;
    res.num = f1.num * f2.den - f2.num * f1.den;
    res.den = f1.den * f2.den;
    return simplify(res);
}

Fraction multiply(Fraction f1, Fraction f2) {
    Fraction res;
    res.num = f1.num * f2.num;
    res.den = f1.den * f2.den;
    return simplify(res);
}

Fraction divide(Fraction f1, Fraction f2) {
    Fraction res;
    res.num = f1.num * f2.den;
    res.den = f1.den * f2.num;
    return simplify(res);
}

Fraction fromString(char* str) {
    Fraction f;
    sscanf(str, "%d/%d", &f.num, &f.den);
    return simplify(f);
}

void printFraction(Fraction f) {
    if (f.den == 1) printf("%d\n", f.num);
    else printf("%d/%d\n", f.num, f.den);
}
