#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 1. 원소 구조체 정의
typedef struct {
    int row;
    int col;
    int value;
} elementArrayList;

// 2. 순차 리스트 구조체
typedef struct {
    elementArrayList* data;
    int size;
    int capacity;
} arrayList;

// 3. 희소행렬 구조체
typedef struct {
    int rows;
    int cols;
    arrayList* al;
} sparseMatrix;

// --- 리스트 관리 함수 ---

arrayList* createArrayList(int size) {
    arrayList* al = (arrayList*)malloc(sizeof(arrayList));
    al->data = (elementArrayList*)malloc(sizeof(elementArrayList) * size);
    al->size = 0;
    al->capacity = size;
    return al;
}

void destroyArrayList(arrayList* al) {
    if (al) { free(al->data); free(al); }
}

int insertArrayList(arrayList* al, int pos, elementArrayList item) {
    if (pos < 0 || pos > al->size) return 0;
    if (al->size == al->capacity) {
        int newCapacity = al->capacity * 2;
        elementArrayList* newData = (elementArrayList*)realloc(al->data, sizeof(elementArrayList) * newCapacity);
        if (newData == NULL) return 0;
        al->data = newData;
        al->capacity = newCapacity;
    }
    for (int i = al->size - 1; i >= pos; i--) al->data[i + 1] = al->data[i];
    al->data[pos] = item;
    al->size++;
    return 1;
}



// 격자 형태로 출력
void printMatrixVisual(sparseMatrix m, const char* title) {
    printf("< %s >\n", title);
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            int foundValue = 0;
            // 리스트를 뒤져서 현재 위치(i, j)에 값이 있는지 확인
            for (int k = 0; k < m.al->size; k++) {
                if (m.al->data[k].row == i && m.al->data[k].col == j) {
                    foundValue = m.al->data[k].value;
                    break;
                }
            }
            printf("%3d ", foundValue);
        }
        printf("\n");
    }
    printf("\n");
}

int transposeSparseMatrix(sparseMatrix a, sparseMatrix* b) {
    int moveCount = 0;
    b->rows = a.cols;
    b->cols = a.rows;
    b->al = createArrayList(a.al->size);

    // 열 우선 탐색 알고리즘
    for (int c = 0; c < a.cols; c++) {
        for (int i = 0; i < a.al->size; i++) {
            if (a.al->data[i].col == c) {
                elementArrayList tItem = { a.al->data[i].col, a.al->data[i].row, a.al->data[i].value };
                insertArrayList(b->al, b->al->size, tItem);
                moveCount++;
            }
        }
    }
    return moveCount;
}

int main() {
    sparseMatrix A, B;
    srand((unsigned int)time(NULL));

    // 1. 원본 행렬 A 생성 및 랜덤 초기화
    A.rows = 10; A.cols = 10;
    A.al = createArrayList(20);

    int count = 0;
    while (count < 20) {
        int r = rand() % 10;
        int c = rand() % 10;
        // 중복 위치 체크 (이미 값이 있으면 다시 뽑기)
        int duplicate = 0;
        for (int i = 0; i < A.al->size; i++) {
            if (A.al->data[i].row == r && A.al->data[i].col == c) {
                duplicate = 1; break;
            }
        }
        if (!duplicate) {
            elementArrayList e = { r, c, rand() % 99 + 1 };
            insertArrayList(A.al, A.al->size, e);
            count++;
        }
    }


    printMatrixVisual(A, "초기 행렬 (A)");

    
    int moves = transposeSparseMatrix(A, &B);

    printMatrixVisual(B, "전치 행렬 (B)");

   
    printf("====================================\n");
    printf("데이터 이동(복사) 총 횟수: %d회\n", moves);
    printf("====================================\n");

   
    destroyArrayList(A.al);
    destroyArrayList(B.al);

    return 0;
}
