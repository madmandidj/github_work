/*
Filename: 		TESTBubbleAndShakeSortVec.c 
Description:	TEST Implementation of Bubble And Shake Sort using Vector API given by Ronny
Created: 		20/07/17
Created by:		Eyal Alon
Last modified: 	21/07/17
Modified by:	Eyal Alon
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"
#include "BubbleAndShakeSortVec.h"


int main()
{
	ADTErr errResult;
	Vector* vec;
	int index;
	int randNum;
	size_t initSize = 10;
	size_t initBlockSize = 2;
	srand (time(NULL));
	vec = VectorCreate(initSize, initBlockSize);
	for (index = 0; index < initSize; ++index)
	{
		randNum = rand() % 100 + 1;
		VectorAdd(vec, randNum);
	}
	printf("Unsorted: \n");
	VectorPrint(vec);
	errResult = BubbleSort(vec);
	printf("Sorted: \n");
	VectorPrint(vec);
	return 0;
}
