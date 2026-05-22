#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArrayStack.h"

int main() {
    ArrayStack* myStack = createArrayStack(100);
    if (myStack == NULL) return -1;

    int choice;
    char inputURL[100];
    int firstActionPassed = 0;

    while (1) {
        printf("\n[1:추가 / 2:제거 / 3:확인]: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            printf("URL 입력:\n");
            scanf("%s", inputURL);

            char* newURL = strdup(inputURL);
            if (newURL != NULL) {
                if (pushArrayStack(myStack, newURL)) {
                    firstActionPassed = 1;
                    printf("현재 창: %s\n", peekArrayStack(myStack));
                }
                else {
                    free(newURL);
                }
            }
            break;

        case 2:
            if (emptyArrayStack(myStack)) break;

            char* poppedURL = popArrayStack(myStack);
            if (poppedURL != NULL) {
                printf("이탈: %s\n", poppedURL);
                free(poppedURL);
            }

            if (emptyArrayStack(myStack)) {
                if (firstActionPassed) {
                    printf("브라우저가 종료되었습니다.\n");
                    destroyArrayStack(myStack);
                    return 0;
                }
            }
            else {
                printf("현재 창: %s\n", peekArrayStack(myStack));
            }
            break;

        case 3:
            if (!emptyArrayStack(myStack)) {
                printf("현재 창: %s\n", peekArrayStack(myStack));
            }
            break;

        default:
            break;
        }
    }
    return 0;
}
