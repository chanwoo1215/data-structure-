#include <stdio.h>
#include <stdlib.h>

#include "polynomial.h"

int main() {
	polynomial* p1;
	p1 = createPolynomial(100);

	addItemPolynomial(p1, (elementType) { 4, 0 });
	addItemPolynomial(p1, (elementType) { 2, 1 });
	addItemPolynomial(p1, (elementType) { 3, 2 });

	printPolynomial(p1);

	polynomial* p2;
	p2 = createPolynomial(100);

	addItemPolynomial(p2, (elementType) { 4, 3 });
	addItemPolynomial(p2, (elementType) { 5, 0 });

	printPolynomial(p2);

	polynomial* p3 = addPolynomial(p1, p2);

	printPolynomial(p3);
}
