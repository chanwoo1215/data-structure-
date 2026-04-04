#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 60


int main_old() {
	int arr[SIZE];
	int sum = 0;
	int max, min;

	srand((unsigned int)time(NULL));

	// 배열 채우기
	for (int i = 0; i < SIZE; i++) {
		arr[i] = rand() % (SIZE + 1);
	}

			
	

	printf(" 정방향 출력 0 -> %d :  \n", SIZE-1);
	for (int i = 0; i < SIZE; i++) {
		printf("%2d ", arr[i]);
		if ((i + 1) % 10 == 0)printf("\n");


	}

	printf("역방향 출력 : %d -> 0\n", SIZE - 1);
	for (int i = SIZE - 1; i >= 0; i--) {
		printf("%2d ", arr[i]);	
		if (i % 10 == 0)printf("\n");
		
	}
	
	max = min = arr[0]; 
	for (int i = 0; i < SIZE; i++) {
		sum += arr[i];
		if (arr[i] > max) max = arr[i];
		if (arr[i] < min) min = arr[i];
	}
	
	printf("\n 총합 : %d\n", sum);
	printf("최대값 : %d\n", max);
	printf("최솟값 : %d\n", min);
		
	return 0;
}
