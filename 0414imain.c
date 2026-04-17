#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"

int main() {
	arrayList* myArrayList;
	myArrayList = createArrayList(2);

	if (sizeArrayList(myArrayList) == 0) {
		printf("Create ok\n");
	}

	// Insert test
	insertArrayList(myArrayList, 0, 10); // ÃÖÃÊ »ðÀÔ
	insertArrayList(myArrayList, 1, 20);
	insertArrayList(myArrayList, 2, 30);
	printf("30 삽입 후 용량 : %d\n", myArrayList->capacity);
	insertArrayList(myArrayList, 0, 5);
	printf("5 삽입 후 용량: %d\n", myArrayList->capacity);

	printf("최종 리스트 : ");
	printArrayList(myArrayList);
	printf("\n");

	printArrayList(myArrayList);

	// Delete test
	deleteArrayList(myArrayList, 0);
	deleteArrayList(myArrayList, 1);
	deleteArrayList(myArrayList, 2);

	printArrayList(myArrayList);
	printf("\n");

	destroyArrayList(myArrayList);
	return 0;
}
