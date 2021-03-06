/*
Filename: 		GenericDoubleLinkedList.c 
Description:	Implementation of generic DLL API
Created: 		06/08/17
Created by:		Eyal Alon
Last modified: 	08/08/17
Modified by:	Eyal Alon
*/

#include <stdio.h>
#include "../../inc/list_itr.h"
/*****
GENERAL MACROS
*****/
#define TRUE 1
#define FALSE 0
#define INVALID -1
#define IS_NULL(A) 		(NULL == (A))
#define IS_FALSE(I)		(NULL == (I))
/*****
LIST MACROS
*****/
#define MAGIC_NUM 0x00002000 /* Generic DLL Magic Num is 0x00002000*/
#define IS_A_LIST(L) 	((L) &&  MAGIC_NUM == (L)->m_magicNum) 
#define IS_LIST_EMPTY(L)	(LIST_FIRST((L)) == LIST_END((L)))
#define LIST_FIRST(L) 	((L)->m_head.m_next)
#define LIST_LAST(L) 	((L)->m_tail.m_prev)
#define LIST_BEGIN(L) 	(&(L)->m_head)
#define LIST_END(L) 	(&(L)->m_tail)
/*****
NODE MACROS
*****/
#define NODE_NEXT(N) 	((N)->m_next)
#define NODE_PREV(N) 	((N)->m_prev)
#define NODE_DATA(N) 	((N)->m_data)
/*****
ITERATOR MACROS
*****/
#define ITR_NEXT(I) (((Node*)(I))->m_next)
#define ITR_PREV(I) (((Node*)(I))->m_prev)
#define ITR_DATA(I) (((Node*)(I))->m_data)


/*
TODO: factorize all functions
TODO: static functions that deal with node allocation shouldnt return errors. errors should be returned from api functions
TODO: change all == checks so that constant is on left side
*/

typedef struct Node Node;

struct Node
{
	void* 	m_data;
	Node*	m_next;
	Node*	m_prev;
};

struct List
{
	size_t m_magicNum;
	Node m_head;
	Node m_tail;
};









/******
LIST CREATE
******/
static void InitList(List* _list)
{
	LIST_FIRST(_list) = LIST_END(_list);
	LIST_LAST(_list) = LIST_BEGIN(_list);
	_list->m_head.m_prev = NULL;
	_list->m_tail.m_next = NULL;
	_list->m_magicNum = MAGIC_NUM;
	return;
}

List* ListCreate(void)
{
	List* list = NULL;
	
	list = malloc(sizeof(List));
	if (IS_NULL(list))
	{
		return NULL;
	}
	InitList(list);
	return list;
}









/******
LIST DESTROY
******/
static void DestroyNodes(List** _list, UserActionFunc _destroyFunc)
{
	Node* curN;

	curN = LIST_FIRST(*_list);
	while(curN != LIST_END(*_list))
	{
		curN = NODE_NEXT(curN);
		if (!IS_NULL(_destroyFunc))
		{
			_destroyFunc(NODE_PREV(curN)->m_data, NULL);
		}
		free(NODE_PREV(curN));
	}
	return;
}

void ListDestroy(List** _list, UserActionFunc _destroyFunc)
{
	if (!IS_NULL(_list) && IS_A_LIST(*_list))
	{
		if (LIST_FIRST(*_list) != LIST_END(*_list))
		{
			DestroyNodes(_list, _destroyFunc);
		}
		(*_list)->m_magicNum = 0;
		free(*_list);
		*_list = NULL;
	}
	return;
}








/******
LIST PUSH HEAD
******/
static Node* CreateNode()
{
	Node* node;
	node = malloc(sizeof(Node));
	return node;
}

static void DoPushHead(List* _list, void* _data, Node* _node)
{
	NODE_DATA(_node) = _data;
	NODE_NEXT(_node) = LIST_FIRST(_list);
	NODE_PREV(_node) = LIST_BEGIN(_list);
	NODE_PREV(_node)->m_next = _node;
	NODE_NEXT(_node)->m_prev = _node;
	return;	
}

ListErrors	ListPushHead(List* _list, void* _data)
{
	Node* node;
	
	if (IS_A_LIST(_list))
	{
		if (!(node = CreateNode()))
		{
			return LIST_ALLOCATION_FAILED;
		}
		DoPushHead(_list, _data, node);
	}
	else
	{
		return LIST_UNINITIALIZED;	
	}
	return LIST_OK;
}









/******
LIST PUSH TAIL
******/
static void DoPushTail(List* _list, void* _data, Node* _node)
{
	NODE_DATA(_node) = _data;
	NODE_PREV(_node) = LIST_LAST(_list);
	NODE_NEXT(_node) = LIST_END(_list);
	NODE_NEXT(_node)->m_prev = _node;
	NODE_PREV(_node)->m_next = _node;
	return;
}

ListErrors	ListPushTail(List* _list, void* _data)
{
	Node* node;
	
	if (IS_A_LIST(_list))
	{
		if (!(node = CreateNode()))
		{
			return LIST_ALLOCATION_FAILED;
		}
		DoPushTail(_list, _data, node);
	}
	else
	{
		return LIST_UNINITIALIZED;	
	}
	return LIST_OK;
}









/******
LIST POP HEAD
******/
static void DoPopHead(List* _list, void* *_data)
{
	Node* freeNode;

	*_data = LIST_FIRST(_list)->m_data;
	freeNode = LIST_FIRST(_list);
	LIST_FIRST(_list)->m_next->m_prev = LIST_BEGIN(_list);
	LIST_FIRST(_list) = LIST_FIRST(_list)->m_next;
	free(freeNode);
	return;
}

ListErrors ListPopHead(List* _list, void* *_data)
{
	if (IS_NULL(_data))
	{
		return LIST_INV_ARG;
	}
	if (IS_A_LIST(_list))
	{
		if (!IS_LIST_EMPTY(_list))
		{
			DoPopHead(_list, _data);
		}
		else
		{
			return LIST_IS_EMPTY;
		}
	}
	else
	{
		return LIST_UNINITIALIZED;	
	}
	return LIST_OK;
}









/******
LIST POP TAIL
******/
static void DoPopTail(List* _list, void* *_data)
{
	Node* freeNode;

	*_data = LIST_LAST(_list)->m_data;
	freeNode = LIST_LAST(_list);
	LIST_LAST(_list)->m_prev->m_next = LIST_END(_list);
	LIST_LAST(_list) = LIST_LAST(_list)->m_prev;
	free(freeNode);
	return;
}

ListErrors ListPopTail(List* _list, void* *_data)
{
	if (IS_NULL(_data))
	{
		return LIST_INV_ARG;
	}
	if (IS_A_LIST(_list))
	{
		if (!IS_LIST_EMPTY(_list))
		{
			DoPopTail(_list, _data);
		}
		else
		{
			return LIST_IS_EMPTY;
		}
	}
	else
	{
		return LIST_UNINITIALIZED;	
	}
	return LIST_OK;
}









/******
LIST COUNT ITEMS
******/
size_t	ListCountItems(const List* _list)
{
	Node* curN;
	size_t nItems = 0;

	if (IS_A_LIST(_list) && !IS_LIST_EMPTY(_list))
	{
		curN = LIST_FIRST(_list);
		while(curN != LIST_END(_list))
		{
			++nItems;
			curN = NODE_NEXT(curN);
		}
	}
	return nItems;
}









/******
LIST IS EMPTY
******/
int		ListIsEmpty(const List* _list)
{
	int result = TRUE;

	if (!IS_A_LIST(_list))
	{
		result = INVALID;
	}
	else if (!IS_LIST_EMPTY(_list))
	{
		result = FALSE;
	}
	return result;
}









/******
LIST FIND FIRST FORWARD
******/
static  ListErrors CheckFindFirstParam(const List* _list, PredicateFunc _predicateFunc, void* *_item)
{
	if (!IS_A_LIST(_list))
	{
		return LIST_UNINITIALIZED;
	}
	if (IS_NULL(_predicateFunc) || IS_NULL(_item) )
	{
		return LIST_INV_ARG;
	}
	if (LIST_FIRST(_list) == LIST_END(_list))
	{
		return LIST_IS_EMPTY;
	}
	return LIST_OK;
}

static int DoFindFirstForward(const List* _list, PredicateFunc _predicateFunc, void* _context, void* *_item)
{
	Node* curN;
	int isFound = FALSE;

	curN = LIST_FIRST(_list);
	while(curN != LIST_END(_list))
	{
		isFound = _predicateFunc(NODE_DATA(curN), _context);
		if (isFound)
		{
			*_item = NODE_DATA(curN);
			return TRUE;
		}
		curN = NODE_NEXT(curN);
	}
	*_item = NULL;
	return FALSE;
}

ListErrors FindFirstForward(const List* _list, PredicateFunc _predicateFunc, void* _context, void* *_item)
{
	ListErrors err;

	if (LIST_OK != (err = CheckFindFirstParam(_list, _predicateFunc, _item)))
	{
		return err;
	}
	else if (DoFindFirstForward(_list, _predicateFunc, _context, _item))
	{
		return LIST_OK;
	}
	return LIST_ITEM_NOT_FOUND;
}









/******
LIST FIND FIRST BACKWARD
******/
static int DoFindFirstBackward(const List* _list, PredicateFunc _predicateFunc, void* _context, void* *_item)
{
	Node* curN;
	int isFound = FALSE;

	curN = LIST_LAST(_list);
	while(curN != LIST_FIRST(_list))
	{
		isFound = _predicateFunc(NODE_DATA(curN), _context);
		if (isFound)
		{
			*_item = NODE_DATA(curN);
			return TRUE;
		}
		curN = NODE_PREV(curN);
	}
	*_item = NULL;
	return FALSE;
}

ListErrors FindFirstBackward(const List* _list, PredicateFunc _predicateFunc, void* _context, void* *_item)
{
	ListErrors err;

	if (LIST_OK != (err = CheckFindFirstParam(_list, _predicateFunc, _item)))
	{
		return err;
	}
	else if (DoFindFirstBackward(_list, _predicateFunc, _context, _item))
	{
		return LIST_OK;
	}
	return LIST_ITEM_NOT_FOUND;
}










/******
LIST FOR EACH 
******/
static size_t CheckListForEachParams(const List* _list, UserActionFunc _action)
{
	if (!IS_A_LIST(_list) || IS_NULL(_action))
	{
		return 0;
	}
	return 1;
}

static size_t DoListForEach(const List* _list, UserActionFunc _action, void* _context)
{
	size_t actionCount = 0;
	int result = 0;
	Node* curN;

	curN = LIST_FIRST(_list);
	while(curN != LIST_END(_list))
	{
		result = _action(NODE_DATA(curN), _context);
		if (result == 0)
		{
			return actionCount;
		}
		++actionCount;
		curN = NODE_NEXT(curN);
	}
	return actionCount;
}

size_t ListForEach(const List* _list, UserActionFunc _action, void* _context)
{
	if (CheckListForEachParams(_list, _action) == 0)
	{
		return 0;
	}
	return DoListForEach(_list, _action, _context);
}




/******
*******
*******
LIST ITERATOR FUNCTIONS 
*******
*******
******/

/******
LIST ITERATOR BEGIN
******/
ListItr ListItrBegin(const List* _list)
{
	if (!IS_A_LIST(_list))
	{
		return NULL;
	}
	return (ListItr) LIST_FIRST(_list);
}








/******
LIST ITERATOR END
******/
ListItr ListItrEnd(const List* _list)
{
	if (!IS_A_LIST(_list))
	{
		return NULL;
	}
	return (ListItr) LIST_END(_list);
}






/******
LIST ITERATOR GET
******/
void* ListItrGet(ListItr _itr)
{
	if (IS_NULL(_itr))
	{
		return NULL;
	}
	if (ITR_NEXT(_itr) == NULL)
	{
		return NULL;
	}
	return ITR_DATA(_itr);
}






/******
LIST ITERATOR SET
******/
void* ListItrSet(ListItr _itr, void* _element)
{
	void* returnData;
	if (IS_NULL(_itr))
	{
		return NULL;
	}
	if (ITR_NEXT(_itr) == NULL)
	{
		return NULL;
	}
	returnData = ITR_DATA(_itr);
	ITR_DATA(_itr) = _element;
	return returnData;
}




/******
LIST ITERATOR PREV
******/
ListItr ListItrPrev(ListItr _itr)
{
	if (IS_NULL(_itr))
	{
		return NULL;
	}
	if (NULL == ITR_PREV(_itr)->m_prev)
	{
		return ((Node*)_itr);
	}
	return ITR_PREV(_itr);
}





/******
LIST ITERATOR NEXT
******/
ListItr ListItrNext(ListItr _itr)
{
	if (IS_NULL(_itr))
	{
		return NULL;
	}
	if (NULL == ITR_NEXT(_itr))
	{
		return ((Node*)_itr);
	}
	return ITR_NEXT(_itr);
}









/******
LIST ITERATOR EQUALS
******/
int ListItrEquals(const ListItr _a, const ListItr _b)
{
	if (IS_NULL(_a) || IS_NULL(_a))
	{
		return FALSE;
	}
	if (_a == _b)
	{
		return TRUE;
	}
	return FALSE;
}









/******
LIST ITERATOR INSERT BEFORE
******/
static void DoItrInsertBefore(ListItr _itr, Node* _node)
{
	NODE_PREV(_node) = ITR_PREV(_itr);
	NODE_NEXT(_node) = ((Node*)_itr);
	ITR_PREV(_itr)->m_next = _node;
	ITR_PREV(_itr) = _node;
	return;
}

ListItr ListItrInsertBefore(ListItr _itr, void* _element)
{
	Node* node;
	
	if (IS_NULL(_itr))
	{
		return NULL;
	}
	if (!(node = CreateNode()))
	{
		return NULL;
	}
	NODE_DATA(node) = _element;
	DoItrInsertBefore(_itr, node);
	return (ListItr) node;
}





/******
LIST ITERATOR REMOVE
******/
static void DoRemove(ListItr _itr)
{
	ITR_NEXT(_itr)->m_prev = ITR_PREV(_itr);
	ITR_PREV(_itr)->m_next = ITR_NEXT(_itr);
	return;
}

void* ListItrRemove(ListItr _itr)
{
	void* data = NULL;
	Node* node;
	
	if (IS_NULL(_itr) || NULL == ITR_NEXT(_itr))
	{
		return NULL;
	}
	data = ITR_DATA(_itr);
	node = _itr;
	DoRemove(_itr);
	free(node);
	return data;
}



