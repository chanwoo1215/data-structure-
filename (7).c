#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	char name[50];
	int id;
	int score;
}Student;

int main() {
	FILE* fp;
	int N;
	Student* students;

	printf("등록할 학생 수는 몇 명인가 : ");
	if (scanf("%d", &N) != 1) return 1;

	students = (Student*)malloc(sizeof(Student) * N);
	if (students == NULL) {
		printf(" 메모리 할당 실패\n");
		return 1;
	}
		fp = fopen("name_list.txt", "r");
		if (fp == NULL) {
			perror("파일열기 실패 원인");
			printf("현재 프로그램 실행 위치에 파일이 없음\n");
			printf("Fail to open\n");
			free(students);
			system("pause");
			return 1;
		}

		srand((unsigned int)time(NULL));

		char buffer[50];
		int count = 0;

		while (count < N && fgets(buffer, sizeof(buffer), fp) != NULL) {
			buffer[strcspn(buffer, "\r\n")] = 0;

			strcpy(students[count].name, buffer);
			students[count].id = count + 1;
			students[count].score = rand() % 101;

			count++;
		}

		printf("\n %-6s   \   %-15s    \  %-5s \n", "학번", "이름", "성적");
		printf("------------------------------------- \n ");
		for (int i = 0; i < count; i++) {
			printf("%03d    | %-15s | %3d점\n",
				students[i].id, students[i].name, students[i].score);
		}

		if (count > 0) {
			int max_idx = 0;
			int min_idx = 0;
			double sum = 0;

			for (int i = 0; i < count; i++) {
				sum += students[i].score;
				if (students[i].score > students[max_idx].score) {
					max_idx = i;
				}
				if (students[i].score < students[min_idx].score) {
					min_idx = i;
				}


			}

			double average = (double)sum / count;



			printf("========================\n");
			printf(" [성적 통계 분석]    \n");
			printf("------------------------\n");
			printf(" = 최고점 : [%03d] %-10s (%d점\n",
				students[max_idx].id, students[max_idx].name, students[max_idx].score);
			printf(" = 최저점: [%03d] %-10s (%d점)\n",
				students[min_idx].id, students[min_idx].name, students[min_idx].score);
			printf("=  평균 점수: %.2f점\n", average);
			printf("======================================\n");
		}
		if (count < N) {
			printf("\n(파일 내 이름 부족으로 %d명만 생성되었습니다.\n", count);
		}

		fclose(fp);
		free(students);

		return 0;
	}
