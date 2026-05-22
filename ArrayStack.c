#include <stdio.h>
#include <stdlib.h>
#include "ArrayStack.h"

ArrayStack* createArrayStack(int size) {
    ArrayStack* re = (ArrayStack*)malloc(sizeof(ArrayStack));
    if (re == NULL) return NULL;
    re->data = (stackElement*)malloc(sizeof(stackElement) * size);
    if (re->data == NULL) {
        free(re);
        return NULL;
    }
    re->top = -1;
    re->size = size;
    return re;
}

int emptyArrayStack(ArrayStack* s) {
    if (s == NULL) return 1;
    return s->top == -1;
}

int fullArrayStack(ArrayStack* s) {
    if (s == NULL) return 1;
    return s->top == s->size - 1;
}

int pushArrayStack(ArrayStack* s, stackElement item) {
    if (s == NULL || fullArrayStack(s)) return 0;
    s->top++;
    s->data[s->top] = item;
    return 1;
}

stackElement popArrayStack(ArrayStack* s) {
    if (s == NULL || emptyArrayStack(s)) return NULL;
    stackElement temp = s->data[s->top];
    s->top--;
    return temp;
}

stackElement peekArrayStack(ArrayStack* s) {
    if (s == NULL || emptyArrayStack(s)) return NULL;
    return s->data[s->top];
}

int printArrayStack(ArrayStack* s) {
    if (s == NULL || emptyArrayStack(s)) return 0;
    return 1;
}

int destroyArrayStack(ArrayStack* s) {
    if (s == NULL) return 0;
    while (!emptyArrayStack(s)) {
        free(popArrayStack(s));
    }
    free(s->data);
    free(s);
    return 1;
}
