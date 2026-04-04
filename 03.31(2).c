#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define  SIZE 5

int main() {
	int magicSquare[SIZE][SIZE] = { 0, };
	int row = 0;
	int col = SIZE / 2;

	for (int num = 1; num <= SIZE * SIZE; num++) {
		magicSquare[row][col] = num;

		if (num % SIZE == 0) {
			row++;
		}
		else
		{
			row--;
			col++;


			if (row < 0) row = SIZE - 1;
			if (col >= SIZE) col = 0;


		}

	}
		printf("%d * %d 마장진 \n", SIZE, SIZE);
		for (int i = 0; i < SIZE; i++) {
			for (int a = 0; a < SIZE; a++) {
				printf("%3d ", magicSquare[i][a]);
			}
			printf("\n");

		}

		
		printf("\n[가로 세트 합계] : \n");
		for (int i = 0; i < SIZE; i++) {
			int sum = 0;
			for (int a = 0; a < SIZE; a++)
			{
				sum += magicSquare[i][a];
				printf("%2d ", magicSquare[i][a]);
				if (a < SIZE - 1)printf(" + ");
			}
			printf(" : 가로 %d행 합 =  %d\n", i + 1, sum);
			
		}

		printf("\n[세로 세트 합계] : \n");
		for (int a = 0; a < SIZE; a++) {
			int sum = 0;
			for (int i = 0; i < SIZE; i++)
			{
				sum += magicSquare[i][a];
				printf("%2d ", magicSquare[i][a]);
				if (i < SIZE - 1)printf(" + ");
			}
			
			printf("세로 %d열 합 : %d\n", a + 1, sum);
		
			}
		

		printf("\n[대각선 세트합계]\n");
		int d1 = 0, d2 = 0;

		for (int i = 0; i < SIZE; i++) {
			d1 += magicSquare[i][i];
			printf("%2d", magicSquare[i][i]);
			if (i < SIZE - 1) printf(" + ");
		}
		printf(" 왼쪽상단에서 오른쪽 하단 대각선 합 : %d\n", d1);
		
		for (int i = 0; i < SIZE; i++) {
			d2 += magicSquare[i][SIZE - 1 - i];
			printf("%2d", magicSquare[i][SIZE - 1 - i]);
			if (i < SIZE - 1)printf(" + ");
		}
		printf(" 오른쪾 상단에서 왼쪽 하단 대각선 합 : %d\n", d2);
	
		
	return 0;	
}
