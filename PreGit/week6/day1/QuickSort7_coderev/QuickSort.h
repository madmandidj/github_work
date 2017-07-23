#ifndef __QUICKSORT_H__
#define __QUICKSORT_H__
/*
Filename: 		QuickSort.h 
Description:	Quick Sort using Vector API given by Ronny
Created: 		21/07/17
Created by:		Eyal Alon
Last modified: 	23/07/17
Modified by:	Eyal Alon
*/
#include "vector.h"
#include "ADTDefs.h"

/*
Desc:	Quick sort iterative	
In:		Vector pointer		
Out:	ERR_OK
Err: 	ERR_NOT_INITIALIZED when _vec == NULL
*/
ADTErr 	QuickSort(Vector* _vec);
/*
Desc:	Quick sort recursive	
In:		Vector pointer		
Out:	ERR_OK
Err: 	ERR_NOT_INITIALIZED when _vec == NULL
*/
ADTErr 	QuickSortRec(Vector* _vec);

#endif /* #ifndef __QUICKSORT_H__ */
