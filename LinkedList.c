#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

LinkedList* createLinkedList() {
    struct linkedList* r;
    r = (struct linkedList*)malloc(sizeof(struct linkedList));
    if (r == NULL) return NULL;

    r->head = (struct pointType*)NULL;
    r->size = 0;

    return r;
}

int destroyLinkedList(LinkedList* li) {
    if (li == NULL) return 0;
    struct pointType* nptr, * mptr;
    for (nptr = li->head; nptr != NULL; nptr = mptr) {
        mptr = nptr->next;
        free(nptr);
    }
    free(li);
    return 1;
}

int isEmptyLinkedList(LinkedList* li) {
    if (li == NULL) return 1;
    return (li->size == 0);
}

int sizeLinkedList(LinkedList* li) {
    if (li == NULL) return 0;
    return li->size;
}

LinkedList* insertFirstLinkedList(LinkedList* li, PointType item) {
    if (li == NULL) return NULL;
    PointType* ptr = (PointType*)malloc(sizeof(PointType));
    if (ptr == NULL) return li;

    *ptr = item;
    ptr->next = li->head;
    li->head = ptr;
    li->size++;

    return li;
}

int printLinkedList(LinkedList* li) {
    if (li == NULL) {
        printf("LinkedList가 존재 X.\n");
        return 0;
    }

    PointType* current = li->head;
    printf("LinkedList:\n");
    printf("LinkedList size: %d\n", li->size);

    
    for (int i = 0; i < li->size; i++) {
        if (current == NULL) break; 
        printf("[%d] x:%d, y:%d, next:%p\n",
            i, current->x, current->y, (void*)current->next);
        current = current->next;
    }
    printf("\n"); 
    return 1;
}

LinkedList* insertLastLinkedList(LinkedList* li, PointType item) {
    if (li == NULL) return NULL;

    
    if (li->head == NULL) {
        return insertFirstLinkedList(li, item);
    }

    PointType* ptr = li->head;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }

    PointType* nPtr = (PointType*)malloc(sizeof(PointType));
    if (nPtr == NULL) return li;
    *nPtr = item;
    nPtr->next = NULL;

    ptr->next = nPtr;
    li->size++;

    return li;
}

LinkedList* deleteAtLinkedList(LinkedList* li, int at) {
    if (li == NULL || li->head == NULL) return li;

   
    if (at >= 0 && at < li->size) {
        if (at == 0) {
            PointType* fPtr = li->head;
            li->head = fPtr->next;
            free(fPtr);
        }
        else {
            PointType* nPtr = li->head;
            for (int i = 0; i < at - 1; i++) {
                if (nPtr->next == NULL) return li; 
                nPtr = nPtr->next;
            }
            PointType* fPtr = nPtr->next;
            if (fPtr != NULL) {
                nPtr->next = fPtr->next;
                free(fPtr);
            }
        }
        li->size--;
    }
    return li;
}

LinkedList* insertItemLinkedList(LinkedList* li, PointType* pre, PointType item) {
    if (li == NULL) return NULL;

    PointType* nPtr = (PointType*)malloc(sizeof(PointType));
    if (nPtr == NULL) return li;
    *nPtr = item;

    if (pre != NULL) {
        nPtr->next = pre->next;
        pre->next = nPtr;
    }
    else {
        nPtr->next = li->head;
        li->head = nPtr;
    }

    li->size++;
    return li;
}

PointType deleteFirstLinkedList(LinkedList* li) {
    if (li != NULL && li->head != NULL && li->size > 0) {
        PointType* tPtr = li->head;
        li->head = tPtr->next;

        PointType temp = *tPtr;
        free(tPtr);

        li->size--;
        return temp;
    }
    else {
        return (PointType) { 0, 0, NULL };
    }
}

PointType deleteLastLinkedList(LinkedList* li) {
   
    if (li == NULL || li->head == NULL || li->size == 0) {
        return (PointType) { 0, 0, NULL };
    }

    PointType* pre = li->head;

    
    if (pre == NULL) {
        li->head = NULL; 
        return (PointType) { 0, 0, NULL };
    }

    
    PointType* iPtr = pre->next;

  
    if (iPtr == NULL) {
        li->head = NULL;
        PointType temp = *pre;
        free(pre);
        li->size--;
        return temp;
    }
   
    else {
        
        while (iPtr->next != NULL) {
            pre = iPtr;
            iPtr = iPtr->next;
        }

        PointType temp = *iPtr;
        pre->next = NULL;
        free(iPtr);
        li->size--;

        return temp;
    }
}
