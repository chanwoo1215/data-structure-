#include <stdio.h>
#include "calculator.h"
#include "ArrayStack.h"

int precedence(char op);

void printCharStack(ArrayStack* s) {
	for (int i = 0; i <= s->top; i++) {
		printf("%c ", s->data[i].operator);
	}
	printf("\n");
}

void printIntStack(ArrayStack* s) {
	for (int i = 0; i <= s->top; i++) {
		printf("%d ", s->data[i].value);
	}
	printf("\n");
}

void printPostfixWithSpace(char* postfix) {
	for (int i = 0; postfix[i] != '\0'; i++) {
		printf("%c ", postfix[i]);
	}
}

int infixToPostfix(char* infix, char* postfix) {
	int j = 0;
	ArrayStack* opStack = createArrayStack(100, CHARACTER);

	printf("Transform:\n");

	for (int i = 0; infix[i] != '\0'; i++) {
		char token = infix[i];

		if (token >= '0' && token <= '9') {
			postfix[j++] = token;
			postfix[j] = '\0';
		}
		else if (token == '+' || token == '-' || token == '*' || token == '/') {
			while (!emptyArrayStack(opStack)) {
				stackElement prevElement = peekArrayStack(opStack);

				if (precedence(token) <= precedence(prevElement.operator)) {
					stackElement popped = popArrayStack(opStack);
					postfix[j++] = popped.operator;
					postfix[j] = '\0';
				}
				else {
					break;
				}
			}
			stackElement item;
			item.operator = token;
			pushArrayStack(opStack, item);
		}
		else if (token == '(') {
			stackElement item;
			item.operator = token;
			pushArrayStack(opStack, item);
		}
		else if (token == ')') {
			while (!emptyArrayStack(opStack)) {
				stackElement prevElement = popArrayStack(opStack);
				if (prevElement.operator == '(') {
					break;
				}
				postfix[j++] = prevElement.operator;
				postfix[j] = '\0';
			}
		}

		printf("- Postfix: ");
		printPostfixWithSpace(postfix);
		printf("| Stack: ");
		printCharStack(opStack);
	}

	while (!emptyArrayStack(opStack)) {
		stackElement prevElement = popArrayStack(opStack);
		postfix[j++] = prevElement.operator;
		postfix[j] = '\0';

		printf("- Postfix: ");
		printPostfixWithSpace(postfix);
		printf("| Stack: ");
		printCharStack(opStack);
	}

	postfix[j] = '\0';

	printf("\n- Result: ");
	printPostfixWithSpace(postfix);
	printf("\n\n");

	return 1;
}

int evalPostfix(char* postfix) {
	ArrayStack* valueStack = createArrayStack(100, INTEGER);
	int i = 0;

	printf("Eval:\n");
	printf("- Postfix: ");
	printPostfixWithSpace(postfix);
	printf("\n");

	while (postfix[i] != '\0') {
		char token = postfix[i];

		if (token >= '0' && token <= '9') {
			stackElement item;
			item.value = (int)(token - '0');
			pushArrayStack(valueStack, item);
		}
		else if (token == '*' || token == '/' || token == '+' || token == '-') {
			int i1 = popArrayStack(valueStack).value;
			int i2 = popArrayStack(valueStack).value;
			stackElement item;

			if (token == '*') item.value = i2 * i1;
			else if (token == '/') item.value = i2 / i1;
			else if (token == '+') item.value = i2 + i1;
			else if (token == '-') item.value = i2 - i1;

			pushArrayStack(valueStack, item);
		}

		printf("- Current: %c | Stack: ", token);
		printIntStack(valueStack);

		i++;
	}

	int finalResult = popArrayStack(valueStack).value;
	printf("\n- Result: %d\n", finalResult);

	return finalResult;
}

int precedence(char op) {
	switch (op) {
	case '*': case '/': return 2;
	case '+': case '-': return 1;
	case '(': case ')': return 0;
	default: return -1;
	}
}
