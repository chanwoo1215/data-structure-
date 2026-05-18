#include "DLinkedList.h"

DLinkedList* createLinkedList() {
    DLinkedList* temp = (DLinkedList*)malloc(sizeof(DLinkedList));
    if (temp == NULL) return NULL;
    temp->head.prev = &(temp->head);
    temp->head.next = &(temp->head);
    temp->size = 0;
    return temp;
}

void destroyLinkedList(DLinkedList* dl) {
    while (!isEmptyLinkedList(dl)) {
        deleteFirstLinkedList(dl);
    }
    free(dl);
}

bool isEmptyLinkedList(DLinkedList* dl) {
    return dl->size == 0;
}

int sizeLinkedList(DLinkedList* dl) {
    return dl->size;
}

int insertFirstLinkedList(DLinkedList* dl, PointType item) {
    return insertItemLinkedList(dl, &(dl->head), item);
}

int insertLastLinkedList(DLinkedList* dl, PointType item) {
    return insertItemLinkedList(dl, dl->head.prev, item);
}

int insertItemLinkedList(DLinkedList* dl, PointType* pre, PointType item) {
    if (pre == NULL) return 0;

    PointType* nPtr = (PointType*)malloc(sizeof(PointType));
    if (nPtr == NULL) return 0;
    *nPtr = item;

    nPtr->prev = pre;
    nPtr->next = pre->next;

    pre->next->prev = nPtr;
    pre->next = nPtr;

    dl->size++;
    return 1;
}

int insertAtLinkedList(DLinkedList* dl, int at, PointType item) {
    if (at < 0 || at > dl->size) return 0;

    PointType* curr = &(dl->head);
    for (int i = 0; i < at; i++) {
        curr = curr->next;
    }
    return insertItemLinkedList(dl, curr, item);
}

PointType deleteFirstLinkedList(DLinkedList* dl) {
    return deleteItemLinkedList(dl, dl->head.next);
}

PointType deleteLastLinkedList(DLinkedList* dl) {
    return deleteItemLinkedList(dl, dl->head.prev);
}

PointType deleteItemLinkedList(DLinkedList* dl, PointType* pos) {
    if (isEmptyLinkedList(dl) || pos == NULL || pos == &(dl->head)) {
        return (PointType) { 0, 0, NULL, NULL };
    }

    PointType returnItem = *pos;

    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;

    free(pos);
    dl->size--;

    return returnItem;
}

PointType deleteAtLinkedList(DLinkedList* dl, int at) {
    if (at < 0 || at >= dl->size) return (PointType) { 0, 0, NULL, NULL };

    PointType* curr = dl->head.next;
    for (int i = 0; i < at; i++) {
        curr = curr->next;
    }
    return deleteItemLinkedList(dl, curr);
}

PointType getItemLinkedList(DLinkedList* dl, PointType* pos) {
    if (pos == NULL || pos == &(dl->head)) return (PointType) { 0, 0, NULL, NULL };
    return *pos;
}

void replaceItemLinkedList(DLinkedList* dl, PointType* pos, PointType item) {
    if (pos == NULL || pos == &(dl->head)) return;
    pos->x = item.x;
    pos->y = item.y;
}

PointType nextItemLinkedList(DLinkedList* dl, PointType* pre) {
    if (pre == NULL || pre->next == &(dl->head)) return (PointType) { 0, 0, NULL, NULL };
    return *(pre->next);
}

PointType* searchByX(DLinkedList* dl, int targetX) {
    PointType* curr = dl->head.next;
    while (curr != &(dl->head)) {
        if (curr->x == targetX) return curr;
        curr = curr->next;
    }
    return NULL;
}

void printLinkedList(DLinkedList* dl) {
    printf("\n[현재 리스트 구조 (크기: %d)]\n", dl->size);
    if (isEmptyLinkedList(dl)) {
        printf("리스트가 비어있음.\n");
        return;
    }
    PointType* curr = dl->head.next;
    int idx = 0;
    while (curr != &(dl->head)) {
        printf("인덱스 [%d] -> 값: %d (주소: %p)\n", idx++, curr->x, (void*)curr);
        curr = curr->next;
    }
    printf("------------------------------------\n");
}
