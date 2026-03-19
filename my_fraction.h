#ifndef MY_FRACTION_H
#define MY_FRACTION_H

typedef struct {
    int num;
    int den;
} Fraction;

Fraction simplify(Fraction f);
Fraction add(Fraction f1, Fraction f2);
Fraction subtract(Fraction f1, Fraction f2);
Fraction multiply(Fraction f1, Fraction f2);
Fraction divide(Fraction f1, Fraction f2);
Fraction fromString(char* str);
void printFraction(Fraction f);

#endif
