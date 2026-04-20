#include <stdio.h>
#include <stdlib.h>
#include "arrayList.h"

// 1. 리스트 생성
arrayList* createArrayList(int size) {
    arrayList* al;
    al = (arrayList*)malloc(sizeof(arrayList));

    al->data = (elementArrayList*)malloc(
        sizeof(elementArrayList) * size);
    al->size = 0;
    al->capacity = size;

    return al;
}

// 2. 리스트 소멸
void destroyArrayList(arrayList* al) {
    free(al->data);
    free(al);
}

// 3. 비어있는지 확인
int isEmptyArrayList(arrayList* al) {
    if (al->size == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

// 4. 가득 찼는지 확인
int isFullArrayList(arrayList* al) {
    if (al->size == al->capacity) {
        return 1;
    }
    else {
        return 0;
    }
}

// 5. 현재 크기 반환
int sizeArrayList(arrayList* al) {
    return al->size;
}

// 6. 데이터 삽입 (양의 정수 체크 및 확장 로직 포함)
int insertArrayList(arrayList* al, int pos, elementArrayList item) {
    // [양의 정수 체크 추가]
    if (item <= 0) {
        printf("오류: 양의 정수(>0)만 삽입할 수 있습니다.\n");
        return 0;
    }

    if (pos < 0 || pos > al->size) {
        printf("오류: 삽입 범위를 벗어났습니다.\n");
        return 0;
    }

    if (isFullArrayList(al)) {
        int newCapacity = al->capacity * 2;
        elementArrayList* newData = (elementArrayList*)realloc(al->data, sizeof(elementArrayList) * newCapacity);

        if (newData == NULL) {
            printf("메모리 재할당 실패!\n");
            return 0;
        }

        al->data = newData;
        al->capacity = newCapacity;
        printf("리스트 확장: %d -> %d\n", al->size, al->capacity);
    }

    for (int i = al->size - 1; i >= pos; i--) {
        al->data[i + 1] = al->data[i];
    }

    al->data[pos] = item;
    al->size++;

    return 1;
}

// 7. 데이터 삭제 (경계 조건 오류 개선)
elementArrayList deleteArrayList(arrayList* al, int pos) {
    // [오류 개선] pos > al->size - 1 보다 명확한 pos >= al->size 사용
    if (pos < 0 || pos >= al->size) {
        printf("오류: 삭제할 위치가 올바르지 않습니다.\n");
        return -1; // 실패 시 -1 반환 (양의 정수 리스트이므로 구분 가능)
    }

    elementArrayList item = al->data[pos];

    for (int i = pos; i < al->size - 1; i++) {
        al->data[i] = al->data[i + 1];
    }

    al->size--;

    return item;
}

// 8. 리스트 전체 초기화 (Clear 기능)
int initArrayList(arrayList* al) {
    if (al == NULL) return 0;
    // 기존 뼈대인 반복문 삭제 방식 유지 또는 효율적인 size = 0 방식 선택
    // 여기서는 뼈대를 존중하여 반복 호출을 유지하되 논리 오류를 수정함
    for (int i = al->size - 1; i >= 0; i--) {
        deleteArrayList(al, i);
    }
    return 1;
}

// 9. 데이터 가져오기
elementArrayList getItemArrayList(arrayList* al, int pos) {
    if (pos < 0 || pos >= al->size) return -1;
    return al->data[pos];
}

// 10. 데이터 변경 (양의 정수 체크 추가)
int replaceItemArrayList(arrayList* al, int pos, elementArrayList item) {
    // [양의 정수 체크 추가]
    if (item <= 0) {
        printf("오류: 양의 정수로만 변경 가능합니다.\n");
        return 0;
    }

    if (pos < 0 || pos >= al->size) {
        printf("오류: 변경할 위치가 올바르지 않습니다.\n");
        return 0;
    }

    al->data[pos] = item;

    return 1;
}

// 11. 리스트 출력 (가독성 개선)
void printArrayList(arrayList* al) {
    printf("순차 리스트 (용량:%d, 크기:%d): ", al->capacity, al->size);
    if (al->size == 0) {
        printf("비어 있음");
    }
    else {
        for (int i = 0; i < al->size; i++) {
            printf("[%d]:%d ", i, al->data[i]);
        }
    }
    printf("\n");
}
