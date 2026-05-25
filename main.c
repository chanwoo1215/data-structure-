#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "calculator.h"

int main() {
	char infix[100];
	char postfix[100];

	printf("입력: ");
	if (scanf("%s", infix) != 1) {
		return 0;
	}

	printf("\n출력:\n");

	infixToPostfix(infix, postfix);

	evalPostfix(postfix);

	return 0;
}
