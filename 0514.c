#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "DLinkedList.h"

void Menu() {
    printf("\n======= 리스트 관리 프로그램 =======\n");
    printf("1. 맨 앞에 추가\n");
    printf("2. 맨 뒤에 추가\n");
    printf("3. 특정 위치에 추가\n");
    printf("4. 특정 위치 삭제\n");
    printf("5. 전체 목록 출력\n");
    printf("6. 리스트 크기 확인\n");
    printf("0. 종료\n");
    printf("====================================\n");
    printf("선택할 메뉴 번호 입력: ");
}

int main() {
    DLinkedList* list = createLinkedList();
    if (list == NULL) {
        printf("리스트 생성 실패.\n");
        return -1;
    }

    int choice, value, index;
    PointType tempItem;

    while (1) {
        Menu();

        if (scanf("%d", &choice) != 1) {
            printf("잘못된 숫자 입력 재입력 : \n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            printf("맨 앞에 삽입할 정수 입력: ");
            scanf("%d", &value);
            insertFirstLinkedList(list, (PointType) { value, 0, NULL, NULL });
            printf("[%d] 가 맨 앞에 추가됨.\n", value);
            break;

        case 2:
            printf("맨 뒤에 삽입할 정수 입력: ");
            scanf("%d", &value);
            insertLastLinkedList(list, (PointType) { value, 0, NULL, NULL });
            printf("[%d] 가 맨 뒤에 추가됨.\n", value);
            break;

        case 3:
            printf("삽입할 위치(index, 0 ~ %d): ", sizeLinkedList(list));
            scanf("%d", &index);
            printf("삽입할 정수 입력: ");
            scanf("%d", &value);

            if (insertAtLinkedList(list, index, (PointType) { value, 0, NULL, NULL })) {
                printf("%d번 위치에 [%d] 삽입 완료.\n", index, value);
            }
            else {
                printf("잘못된 index 범위 (0부터 %d까지만 가능)\n", sizeLinkedList(list));
            }
            break;

        case 4:
            if (isEmptyLinkedList(list)) {
                printf("리스트가 비어 있어 삭제할 수 없음.\n");
                break;
            }
            printf("삭제할 위치(index, 0 ~ %d): ", sizeLinkedList(list) - 1);
            scanf("%d", &index);

            tempItem = deleteAtLinkedList(list, index);
            if (tempItem.next != NULL || tempItem.prev != NULL) {
                printf("지워진 %d번 위치의 노드 값: [%d]\n", index, tempItem.x);
            }
            else {
                printf("잘못된 index 범위 (0부터 % d까지만 가능)\n", sizeLinkedList(list) - 1);
            }
            break;

        case 5:
            printLinkedList(list);
            break;

        case 6:
            printf("현재 저장된 노드의 총 개수: %d개\n", sizeLinkedList(list));
            break;

        case 0:
            printf("모든 메모리 해제\n");
            destroyLinkedList(list);
            return 0;

        default:
            printf("0~6번 사이의 지정된 번호만 선택.\n");
            break;
        }
    }
}
