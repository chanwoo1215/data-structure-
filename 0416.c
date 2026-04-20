#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "arrayList.h"


int main() {
    int choice;
    int pos;
    elementArrayList item;

    // 리스트 초기화 (초기 용량 5로 설정)
    arrayList* myL = createArrayList(5);

    while (1) {
        printf("\n--- 양의 정수 리스트 관리 프로그램 ---\n");
        printf("1. 추가 (Insert)\n");
        printf("2. 삭제 (Delete)\n");
        printf("3. 변경 (Replace)\n");
        printf("4. 출력 (Print)\n");
        printf("5. 전체 삭제 (Clear)\n");
        printf("0. 종료\n");
        printf("메뉴 선택: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
        case 1: // 1. 양의 정수와 위치를 입력 받아 추가
            printf("추가할 위치와 양의 정수 입력 (예: 0 10): ");
            scanf("%d %d", &pos, &item);
            if (insertArrayList(myL, pos, item)) {
                printf("성공적으로 추가되었습니다.\n");
            }
            else {
                printf("추가 실패! (양수 여부 및 인덱스 확인)\n");
            }
            break;

        case 2: // 2. 위치를 입력 받아 삭제
            printf("삭제할 위치(인덱스) 입력: ");
            scanf("%d", &pos);
            item = deleteArrayList(myL, pos);
            if (item > 0) {
                printf("삭제된 항목: %d\n", item);
            }
            else {
                printf("삭제 실패! 위치를 확인하세요.\n");
            }
            break;

        case 3: // 3. 양의 정수와 위치를 입력 받아 변경
            printf("변경할 위치와 새로운 양의 정수 입력 (예: 1 50): ");
            scanf("%d %d", &pos, &item);
            if (replaceItemArrayList(myL, pos, item)) {
                printf("성공적으로 변경되었습니다.\n");
            }
            else {
                printf("변경 실패! (양수 여부 및 인덱스 확인)\n");
            }
            break;

        case 4: // 4. 리스트를 위치(인덱스)와 함께 프린트
            printArrayList(myL);
            break;

        case 5: // 5. 모든 값을 삭제
            initArrayList(myL);
            printf("리스트의 모든 데이터를 삭제(초기화)했습니다.\n");
            break;

        default:
            printf("잘못된 메뉴 번호입니다.\n");
        }
    }

    destroyArrayList(myL);
    printf("프로그램을 종료합니다.\n");
    return 0;
}
