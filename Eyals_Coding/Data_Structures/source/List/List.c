#include <stdlib.h>
#include "../../inc/List.h"

#define IS_A_LIST(L) ((L))
#define IS_LIST_EMPTY(L) (LIST_FIRST(L) == LIST_END(L))
#define LIST_FIRST(L) ((L)->m_head.m_next)
#define LIST_LAST(L) ((L)->m_tail.m_prev)
#define LIST_BEGIN(L) (&(L)->m_head)
#define LIST_END(L) (&(L)->m_tail)

#define NODE_DATA(N) ((N)->m_data)
#define NODE_NEXT(N) ((N)->m_next)
#define NODE_PREV(N) ((N)->m_prev)

#define ITR_DATA(I) ((Node*)(I)->m_data)
#define ITR_NEXT(I) ((Node*)(I)->m_next)
#define ITR_PREV(I) ((Node*)(I)->m_prev)

/*typedef struct Node Node;*/
/*typedef Node* ListItr;*/

struct Node
{
    void* m_data;
    Node* m_next;
    Node* m_prev;
};

struct List
{
    Node m_head;
    Node m_tail;
};

List* ListCreate()
{
    List* list;
    if (!(list = malloc(sizeof(List))))
    {
        return NULL;
    }
    LIST_FIRST(list) = LIST_END(list);
    LIST_LAST(list) = LIST_BEGIN(list);
    list->m_head.m_prev = NULL;
    list->m_tail.m_next= NULL;
    return list;
}

void ListDestroy(List* _list, ElementFunc _elemDestroyFunc)
{
    ListItr iterator;
    if (!_list)
    {
        return;
    }
    if (_elemDestroyFunc)
    {
        iterator = LIST_FIRST(_list);
        while(LIST_END(_list) != iterator)
        {
            _elemDestroyFunc(ITR_DATA(iterator));
            iterator = ITR_NEXT(iterator);
        }
    }
    free(_list);
}

ADTErr ListPushHead(List* _list, void* _element)
{
    Node* newNode;
    if (!_list || !_element)
    {
        return ERR_INVARG;
    }
    if (!(newNode = malloc(sizeof(Node))))
    {
        return ERR_NOMEM;
    }
    NODE_DATA(newNode) = _element;
    NODE_NEXT(newNode) = LIST_FIRST(_list);
    NODE_PREV(newNode) = LIST_BEGIN(_list);
    NODE_PREV(LIST_FIRST(_list)) = newNode;
    LIST_FIRST(_list) = newNode;
    return ERR_OK;
}

ADTErr ListPopHead(List* _list, void** _removedElement)
{
    if (!_list || !_removedElement)
    {
        return ERR_INVARG;
    }
	*_removedElement = NODE_DATA((LIST_FIRST(_list)));
    NODE_PREV(NODE_NEXT(LIST_FIRST(_list))) = LIST_BEGIN(_list);
    LIST_FIRST(_list) = NODE_NEXT(LIST_FIRST(_list));
    return ERR_OK;
}

ADTErr ListPushTail(List* _list, void* _element)
{
	Node* newNode;
	if (!_list || !_element)
    {
        return ERR_INVARG;
    }
    if (!(newNode = malloc(sizeof(Node))))
    {
        return ERR_NOMEM;
    }
    NODE_DATA(newNode) = _element;
    NODE_NEXT(newNode) = LIST_END(_list);
    NODE_PREV(newNode) = LIST_LAST(_list);
    NODE_NEXT(LIST_LAST(_list)) = newNode;
    LIST_LAST(_list) = newNode;
    return ERR_OK;
}

ADTErr ListPopTail(List* _list, void** _removedElement)
{
	if (!_list || !_removedElement)
    {
        return ERR_INVARG;
    }
	*_removedElement = NODE_DATA((LIST_LAST(_list)));
    NODE_NEXT(NODE_PREV(LIST_LAST(_list))) = LIST_END(_list);
    LIST_LAST(_list) = NODE_PREV(LIST_LAST(_list));
    return ERR_OK;
}

ADTErr InsertAfter(List* _list, ListItr _itr, void* _element)
{
	if (!_list || !_element)
    {
        return ERR_INVARG;
    }
    /*
    pseudo:
    if (itr is on tail || on last element)
    {
    	PushTail
    }
    if (itr is on head)
    {
    	PushHead
    }
    Do Insert
    
    
    */
}

void ListPrint(List* _list, ElementFunc _elemPrintFunc)
{
    ListItr iterator;
    if (!_list || !_elemPrintFunc)
    {
        return;
    }
    iterator = LIST_FIRST(_list);
    while(LIST_END(_list) != iterator)
    {
        _elemPrintFunc(ITR_DATA(iterator));
        iterator = ITR_NEXT(iterator);
    }
}

/*TODO: ForEach() */

