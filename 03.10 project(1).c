#define _CRT_SECURE_NO_WARNINGS // Visual Studio 보안 경고 방지
#include <stdio.h>
#include <limits.h>

int main() {
    // 8비트 정수(char) 기준 최솟값 확인
    // 2의 보수 시스템: CHAR_MIN은 -128
    // 1의 보수 시스템: CHAR_MIN은 -127

    if (CHAR_MIN == -128) {
        printf("2의 보수입니다.\n");
    } 
    else if (CHAR_MIN == -127) {
        printf("1의 보수입니다.\n");
    } 
    else {
        printf("알 수 없는 방식입니다.\n");
    }

    // 프로그램이 바로 꺼지는 것 방지
    // return 0; 앞에 위치시킴.
    system("pause"); 

    return 0;
}
