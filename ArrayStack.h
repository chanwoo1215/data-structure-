#ifndef ARRAYSTACK_H
#define ARRAYSTACK_H

typedef char* stackElement;

typedef struct arrayStack {
    stackElement* data;
    int top;
    int size;
} ArrayStack;

ArrayStack* createArrayStack(int size);
int emptyArrayStack(ArrayStack* s);
int fullArrayStack(ArrayStack* s);
int pushArrayStack(ArrayStack* s, stackElement item);
stackElement popArrayStack(ArrayStack* s);
stackElement peekArrayStack(ArrayStack* s);
int printArrayStack(ArrayStack* s);
int destroyArrayStack(ArrayStack* s);

#endif
