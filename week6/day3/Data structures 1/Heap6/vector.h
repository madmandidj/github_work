#ifndef __VECTOR_H__
#define __VECTOR_H__
#include "ADTDefs.h"

typedef struct Vector Vector;
Vector* VectorCreate(size_t _initialSize, size_t _extensionBblockSize);
void 	VectorDestroy(Vector* _vector);
ADTErr  VectorAdd(Vector *_vector,  int  _item);
ADTErr  VectorDelete(Vector *_vector,  int* _item);
ADTErr 	VectorGet(Vector *_vector, size_t _index, int *_item);
ADTErr 	VectorSet(Vector *_vector, size_t _index, int  _item);
ADTErr 	VectorItemsNum(Vector *_vector, int*  _numOfItems);
void 	VectorPrint(Vector *_vector);

#endif /*#ifndef __VECTOR_H__*/





