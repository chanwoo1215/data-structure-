#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DATA_SIZE 100
#define SEARCH_KEYS_SIZE 50

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// 
TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


TreeNode* insertBST(TreeNode* root, int data, int* buildCompCount) {
    if (root == NULL) {
        return createNode(data);
    }

    (*buildCompCount)++; // 기존 노드와 비교할 때마다 횟수 증가

    if (data < root->data) {
        root->left = insertBST(root->left, data, buildCompCount);
    }
    else if (data > root->data) {
        root->right = insertBST(root->right, data, buildCompCount);
    }

    return root;
}

// 1. 배열 순차 탐색 (Sequential Search)
int sequentialSearch(int arr[], int size, int key, int* compCount) {
    *compCount = 0;
    for (int i = 0; i < size; i++) {
        (*compCount)++;
        if (arr[i] == key) {
            return i; 
        }
    }
    return -1; 
}

// 2. 이진 탐색 트리 탐색 (BST Search)
TreeNode* searchBST(TreeNode* root, int key, int* compCount) {
    TreeNode* current = root;
    *compCount = 0;

    while (current != NULL) {
        (*compCount)++;
        if (key == current->data) {
            return current; 
        }
        else if (key < current->data) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return NULL; 
}

int main() {
    int array[DATA_SIZE];
    bool isUsed[1001] = { false };
    TreeNode* root = NULL;

    srand((unsigned int)time(NULL));

    
    // [1] 중복 없는 0~1000 사이 정수 100개 생성 및 배열/BST 저장
    
    int bstBuildCompCount = 0;
    int count = 0;

    while (count < DATA_SIZE) {
        int num = rand() % 1001;
        if (isUsed[num]) continue;

        isUsed[num] = true;
        array[count] = num; 

        
        root = insertBST(root, num, &bstBuildCompCount);
        count++;
    }

    // [출력 1] 생성된 100개의 정수
    printf("=======================================================================\n");
    printf("[1] 임의 생성된 100개의 정수 (발생 순서대로 배열 저장)\n");
    printf("=======================================================================\n");
    for (int i = 0; i < DATA_SIZE; i++) {
        printf("%4d ", array[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }

    // [출력 2] BST 생성 과정 총 비교 횟수
    printf("\n=======================================================================\n");
    printf("[2] 이진 탐색 트리(BST) 생성 비용\n");
    printf("=======================================================================\n");
    printf(" - 100개 정수 삽입 시 발생한 총 비교 횟수 : %d 회\n", bstBuildCompCount);

    
    // [2] 50개 탐색 대상(Search Key) 임의 생성
    
    int searchKeys[SEARCH_KEYS_SIZE];
    for (int i = 0; i < SEARCH_KEYS_SIZE; i++) {
        searchKeys[i] = rand() % 1001;
    }

    // [출력 3] 생성된 50개 탐색 대상
    printf("\n=======================================================================\n");
    printf("[3] 임의 생성된 50개의 탐색 대상 (Search Keys)\n");
    printf("=======================================================================\n");
    for (int i = 0; i < SEARCH_KEYS_SIZE; i++) {
        printf("%4d ", searchKeys[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }

    
    // [3] 50회 탐색 수행 및 개별/그룹 통계 측정
    
    int totalSeqComp = 0, totalBSTComp = 0;

    // 추가 통계 변수 (성공/실패 분리 및 최소/최대 비교 횟수)
    int successCount = 0, failCount = 0;
    int seqSuccessComp = 0, seqFailComp = 0;
    int bstSuccessComp = 0, bstFailComp = 0;

    int minSeqComp = DATA_SIZE + 1, maxSeqComp = 0;
    int minBSTComp = DATA_SIZE + 1, maxBSTComp = 0;

    printf("\n=======================================================================\n");
    printf("[4] 50개 탐색 대상별 개별 탐색 수행 결과\n");
    printf("=======================================================================\n");
    printf("%-6s | %-10s | %-10s | %-15s | %-15s\n",
        "번호", "탐색 대상 값", "성공/실패", "순차탐색 비교횟수", "BST탐색 비교횟수");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < SEARCH_KEYS_SIZE; i++) {
        int key = searchKeys[i];
        int seqComp = 0, bstComp = 0;

        int seqIdx = sequentialSearch(array, DATA_SIZE, key, &seqComp);
        TreeNode* bstRes = searchBST(root, key, &bstComp);

        bool isFound = (seqIdx != -1);

        // 전체 총계 누적
        totalSeqComp += seqComp;
        totalBSTComp += bstComp;

        // 최댓값/최솟값 갱신
        if (seqComp < minSeqComp) minSeqComp = seqComp;
        if (seqComp > maxSeqComp) maxSeqComp = seqComp;
        if (bstComp < minBSTComp) minBSTComp = bstComp;
        if (bstComp > maxBSTComp) maxBSTComp = bstComp;

        // 성공 / 실패 그룹별 분리 집계 (추가 통계)
        if (isFound) {
            successCount++;
            seqSuccessComp += seqComp;
            bstSuccessComp += bstComp;
        }
        else {
            failCount++;
            seqFailComp += seqComp;
            bstFailComp += bstComp;
        }

        printf("[%02d]   | %-12d | %-10s | %-17d | %-15d\n",
            i + 1, key, isFound ? "성공 (O)" : "실패 (X)", seqComp, bstComp);
    }

    // =========================================================================
    // [4] 최종 통계 및 종합 성능 분석 출력
    // =========================================================================
    double avgSeqTotal = (double)totalSeqComp / SEARCH_KEYS_SIZE;
    double avgBSTTotal = (double)totalBSTComp / SEARCH_KEYS_SIZE;

    printf("=======================================================================\n");
    printf("[5] 탐색 결과 기본 통계 요약\n");
    printf("=======================================================================\n");
    printf("1. 순차 탐색 (Sequential Search)\n");
    printf("   - 총 비교 횟수   : %d 회\n", totalSeqComp);
    printf("   - 평균 비교 횟수 : %.2f 회\n", avgSeqTotal);
    printf("2. 이진 탐색 트리 (BST Search)\n");
    printf("   - 총 비교 횟수   : %d 회\n", totalBSTComp);
    printf("   - 평균 비교 횟수 : %.2f 회\n", avgBSTTotal);

    // 심화 통계 정보
    printf("\n=======================================================================\n");
    printf("[6] 추가 성능 분석 심화 통계 정보 (Advanced Statistics)\n");
    printf("=======================================================================\n");
    printf("1. 탐색 성공/실패 분포: 성공 %d건, 실패 %d건 (성공률: %.1f%%)\n",
        successCount, failCount, ((double)successCount / SEARCH_KEYS_SIZE) * 100);

    printf("2. 성공/실패 조건별 평균 비교 횟수:\n");
    if (successCount > 0) {
        printf("   - 탐색 성공 시 -> 순차 탐색: %.2f회 | BST 탐색: %.2f회\n",
            (double)seqSuccessComp / successCount, (double)bstSuccessComp / successCount);
    }
    if (failCount > 0) {
        printf("   - 탐색 실패 시 -> 순차 탐색: %.2f회 | BST 탐색: %.2f회\n",
            (double)seqFailComp / failCount, (double)bstFailComp / failCount);
    }

    printf("3. 단일 탐색 최고/최저 비교 횟수 범주 (Min ~ Max):\n");
    printf("   - 순차 탐색 : %d회 ~ %d회\n", minSeqComp, maxSeqComp);
    printf("   - BST 탐색  : %d회 ~ %d회\n", minBSTComp, maxBSTComp);

    printf("4. 트리 구축 비용(Overhead) 포함 종합 비용 분석:\n");
    printf("   - 순차 탐색 종합 비용 (생성 0회 + 50회 탐색 %d회) = %d 회\n",
        totalSeqComp, totalSeqComp);
    printf("   - BST 탐색 종합 비용  (생성 %d회 + 50회 탐색 %d회) = %d 회\n",
        bstBuildCompCount, totalBSTComp, bstBuildCompCount + totalBSTComp);

    double breakEvenPoint = (double)bstBuildCompCount / (avgSeqTotal - avgBSTTotal);
    printf("   - 손익분기점 (Break-even Point) : 약 %.1f 회 탐색 시점\n", breakEvenPoint);
    printf("     (* 약 %.0f회 이상의 탐색이 이루어질 때부터 BST 초기 구축 비용이 회수됨)\n", breakEvenPoint);
    printf("=======================================================================\n");

    return 0;
}
