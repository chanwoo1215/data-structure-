#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

int main() {
    
    LinkedList* myList = createLinkedList();
    if (myList == NULL) {
        printf("리스트 생성 실패!\n");
        return -1;
    }

    int choice, at;
    PointType item;

    while (1) {
        printf("\n======= 관리 프로그램 목록  =======\n");
        printf("1. 맨 앞에 추가\n");
        printf("2. 맨 뒤에 추가\n");
        printf("3. 특정 위치에 추가\n");
        printf("4. 특정 위치 삭제\n");
        printf("5. 전체 목록 출력\n");
        printf("6. 리스트 크기 확인\n");
        printf("0. 프로그램 종료\n");
        printf("=========================================\n");
        printf("메뉴 번호를 입력 : ");

        if (scanf("%d", &choice) != 1) {
            printf("숫자 입력 : \n");
            while (getchar() != '\n'); 
            continue;
        }

      
        item.next = NULL;

        switch (choice) {
        case 1:
            printf("[맨 앞 추가] x와 y 좌표를 입력 : (예: 10 20): ");
            scanf("%d %d", &item.x, &item.y);
            myList = insertFirstLinkedList(myList, item);
            printf("(%d, %d)가 맨 앞에 추가되었음.\n", item.x, item.y);
            break;

        case 2:
            printf("[맨 뒤 추가] x와 y 좌표를 입력 : (예: 30 40): ");
            scanf("%d %d", &item.x, &item.y);
            myList = insertLastLinkedList(myList, item);
            printf("(%d, %d)가 맨 뒤에 추가되었음.\n", item.x, item.y);
            break;

        case 3:
            printf("[특정 위치 추가] 추가할 위치(인덱스 0부터 시작) 입력: ");
            scanf("%d", &at);

            if (at < 0 || at > myList->size) {
                printf("오류: 올바르지 않은 범위입니다. (현재 0 ~ %d 가능)\n", myList->size);
                break;
            }

            printf("추가할 x와 y 좌표를 입력하세요: ");
            scanf("%d %d", &item.x, &item.y);

            if (at == 0) {
                myList = insertItemLinkedList(myList, NULL, item);
            }
            else {
                PointType* pre = myList->head;
                for (int i = 0; i < at - 1; i++) {
                    pre = pre->next;
                }
                myList = insertItemLinkedList(myList, pre, item);
            }
            printf("위치 %d에 (%d, %d)가 추가되었습니다.\n", at, item.x, item.y);
            break;

        case 4:
            printf("[특정 위치 삭제] 삭제할 위치(인덱스 0부터 시작) 입력: ");
            scanf("%d", &at);
            if (at < 0 || at >= myList->size) {
                printf("오류: 삭제할 수 없는 범위입니다. (현재 0 ~ %d 가능)\n", myList->size - 1);
                break;
            }
            myList = deleteAtLinkedList(myList, at);
            printf("위치 %d의 노드가 삭제되었습니다.\n", at);
            break;

        case 5:
            printLinkedList(myList);
            break;

        case 6:
            printf("현재 리스트의 크기(노드 개수): %d\n", sizeLinkedList(myList));
            break;

        case 0:
            destroyLinkedList(myList); 
            printf("프로그램 종료\n");
            return 0;

        default:
            printf("잘못된 입력입니다. 0~6 사이의 숫자를 입력해 주세요.\n");
        }
    }
    return 0;
}
