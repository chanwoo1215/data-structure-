#ifndef Dlinked_list_h
#define Dlinked_list_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct pointType {
    int x;
    int y;
    struct pointType* prev;
    struct pointType* next;
} PointType;

typedef struct dLinkedList {
    PointType head;
    int size;
} DLinkedList;

DLinkedList* createLinkedList();
void destroyLinkedList(DLinkedList* dl);

bool isEmptyLinkedList(DLinkedList* dl);
int sizeLinkedList(DLinkedList* dl);

int insertFirstLinkedList(DLinkedList* dl, PointType item);
int insertLastLinkedList(DLinkedList* dl, PointType item);
int insertItemLinkedList(DLinkedList* dl, PointType* pre, PointType item);
int insertAtLinkedList(DLinkedList* dl, int at, PointType item);

PointType deleteFirstLinkedList(DLinkedList* dl);
PointType deleteLastLinkedList(DLinkedList* dl);
PointType deleteItemLinkedList(DLinkedList* dl, PointType* pos);
PointType deleteAtLinkedList(DLinkedList* dl, int at);

PointType getItemLinkedList(DLinkedList* dl, PointType* pos);
void replaceItemLinkedList(DLinkedList* dl, PointType* pos, PointType item);
PointType nextItemLinkedList(DLinkedList* dl, PointType* pre);

PointType* searchByX(DLinkedList* dl, int targetX);

void printLinkedList(DLinkedList* dl);

#endif
