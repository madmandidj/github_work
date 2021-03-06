

#include <stdlib.h> /* malloc, free */
#include <assert.h>
#include "bin_tree.h"
/*****
GENERAL MACROS
*****/
#define TRUE 			1
#define FALSE 			0
#define INVALID 		-1
#define IS_NULL(A) 		(NULL == (A))
#define IS_FALSE(I)		(NULL == (I))
/*****
TREE MACROS
*****/
#define MAGIC_NUM 		0x00003000 /* Generic BST Magic Num is 0x00003000*/
#define IS_A_TREE(T) 	((T) &&  MAGIC_NUM == (T)->m_magicNum)
#define TREE_ROOT(T) 	((T)->m_sentinel->m_left)
#define TREE_END(T) 	((T)->m_sentinel)
/*****
NODE MACROS
*****/
#define NODE_ITEM(N) 	((N)->m_item)
#define NODE_FATHER(N) 	((N)->m_father)
#define NODE_LEFT(N) 	((N)->m_left)
#define NODE_RIGHT(N) 	((N)->m_right)

typedef struct Node Node;
struct Node
{
	void* m_item;
	Node* m_left;
	Node* m_right;
	Node* m_father;
};

struct BSTree
{
	int m_magicNum; /* magic num should be first so that if someone "dores" than the magicnum will be affected first. */
	Node* m_sentinel; /* Change to sentinel */
    LessComparator m_lessFunction;
};

/*****
BSTree CREATE
*****/
static BSTree* DoBSTreeCreate()
{
	BSTree* tree;

	tree = malloc(sizeof(BSTree));
	if (IS_NULL(tree))
	{
		return NULL;
	}
	tree->m_sentinel = malloc(sizeof(Node));
	if (IS_NULL(tree->m_sentinel))
	{
		return NULL;
	}
	return tree;
}

static void BSTreeInit(BSTree* _tree, LessComparator _less)
{
	assert(!IS_NULL(_tree) && !IS_NULL(_less));
	_tree->m_sentinel->m_father = NULL;
	_tree->m_sentinel->m_right = NULL;
	_tree->m_sentinel->m_left = NULL;
	_tree->m_lessFunction = _less;
	_tree->m_magicNum = MAGIC_NUM;
	return;
}

BSTree* BSTreeCreate(LessComparator _less)
{
    BSTree* tree;
	if (IS_NULL(_less))
	{
		return NULL;
	}
	if (!(tree = DoBSTreeCreate()))
	{
		return NULL;
	}
	BSTreeInit(tree, _less);
	return tree;
}







/*****
BSTree DESTROY
*****/
static void TreeDestroyRec(Node* _node, DestroyFunction _destroyFunction)
{
	if (IS_NULL(_node))
	{
		return;
	}
	TreeDestroyRec(_node->m_left, _destroyFunction);
	TreeDestroyRec(_node->m_right, _destroyFunction);
	if (!IS_NULL(_destroyFunction)) /* to improve efficiency make two functions .. one with dstroy and one without */
	{
		_destroyFunction(_node);
	}
	free(_node);
}

void BSTreeDestroy(BSTree* _tree, DestroyFunction _destroyFunction)
{
	Node* node;
	if (IS_A_TREE(_tree))
	{
		node = TREE_ROOT(_tree);
		TreeDestroyRec(node, _destroyFunction);
		_tree->m_magicNum = 0;
		free(_tree);
	}
	return;
}







/*****
BSTree INSERT
*****/
static Node* CreateNodeWithItem(void* _item)
{
	Node* node;

	assert(!IS_NULL(_item));
	node = malloc(sizeof(Node));
	if (IS_NULL(node))
	{
		return NULL;
	}
	NODE_ITEM(node) = _item;
	NODE_FATHER(node) = NULL;
	NODE_LEFT(node) = NULL;
	NODE_RIGHT(node) = NULL;
	return node;
}

static Node* BubbleDownRec(BSTree* _tree, Node* _node, void* _item)
{
	Node* foundNode = NULL;

	assert(IS_A_TREE(_tree) && !IS_NULL(_node) && !IS_NULL(_item));
	if (_tree->m_lessFunction(_item, _node->m_item))
	{
		if (_node->m_left == NULL)
		{
			foundNode = _node;
		}
		else
		{
			foundNode = BubbleDownRec(_tree, _node->m_left, _item);
		}
	}
	else
	{
		if (_node->m_right == NULL)
		{
			foundNode = _node;
		}
		else
		{
			foundNode = BubbleDownRec(_tree, _node->m_right, _item);
		}
	}
	return foundNode;
}

static Node* DoInsertNode(BSTree* _tree, Node* _node, void* _item)
{
	assert(IS_A_TREE(_tree) && !IS_NULL(_node) && !IS_NULL(_item));
	if (_tree->m_lessFunction(_item, _node->m_item))
	{
		if (!(_node->m_left = CreateNodeWithItem(_item)))
		{
			return NULL;
		}
		else
		{
			_node->m_left->m_father = _node;
			_node->m_left->m_left = NULL;
			_node->m_left->m_right = NULL;
			return _node->m_left;
		}
	}
	else 
	{
		if (!(_node->m_right = CreateNodeWithItem(_item)))
		{
			return NULL;
		}
		else
		{
			_node->m_right->m_father = _node; 
			_node->m_right->m_left = NULL;
			_node->m_right->m_right = NULL;
			return _node->m_right;
		}
	}
	return NULL;
}

BSTreeItr BSTreeInsert(BSTree* _tree, void* _item)
{
	Node* node = NULL;

	if (!IS_A_TREE(_tree) || IS_NULL(_item))
	{
		return NULL;
	}
	if (NULL == TREE_ROOT(_tree))
	{
		if (!(TREE_ROOT(_tree) = CreateNodeWithItem(_item)))
		{
			return NULL;
		}
		TREE_ROOT(_tree)->m_father = _tree->m_sentinel;
		TREE_ROOT(_tree)->m_left = NULL;
		TREE_ROOT(_tree)->m_right = NULL;
	}
	else if (!(node = BubbleDownRec(_tree, TREE_ROOT(_tree), _item)))
	{
		return NULL;
	}
	else 
	{
		if(!(node = DoInsertNode(_tree, node, _item)))
		{
			return NULL;
		}
	}
	return (BSTreeItr)node;
}






/*****
BSTree FIND FIRST
*****/
/*
STreeItr BSTreeFindFirst(const BSTree* _tree, PredicateFunction _predicate, void* _context)
{
	if (!IS_A_TREE(_tree) || IS_NULL(_item))
	{
		return NULL;
	}
}
*/







/*****
BSTree ITR BEGIN
*****/
BSTreeItr BSTreeItrBegin(const BSTree* _tree)
{
	Node* node;
	if (!IS_A_TREE(_tree))
	{
		return NULL;
	}
	node = TREE_ROOT(_tree);
	while (NULL != NODE_LEFT(node))
	{
		node = NODE_LEFT(node);
	}
	return (BSTreeItr) node;
}






/*****
BSTree ITR END
*****/
BSTreeItr BSTreeItrEnd(const BSTree* _tree)
{
	if (!IS_A_TREE(_tree))
	{
		return NULL;
	}
	return (BSTreeItr)_tree->m_sentinel;
}





/*****
BSTree ITR EQUALS
*****/

/*****
BSTree ITR NEXT
*****/
static BSTreeItr FindDeepestLeft(BSTreeItr _it)
{
	while(NULL != ((Node*)_it)->m_left)
	{
		_it = ((Node*)_it)->m_left;
	}
	return _it;
}

static BSTreeItr FindFirstRightFather(BSTreeItr _it)
{
	while((Node*)_it != ((Node*)_it)->m_father->m_left)
	{
		_it = ((Node*)_it)->m_father;
	}
	_it = ((Node*)_it)->m_father;
	return _it;
}

BSTreeItr BSTreeItrNext(BSTreeItr _it)
{

	if (IS_NULL(_it)) 
	{
		return NULL;
	}
	if (((Node*)_it)->m_right != NULL)
	{
		_it = FindDeepestLeft(((Node*)_it)->m_right);
	}
	else
	{
		_it = FindFirstRightFather(_it);
	}
	return _it;
}





/*****
BSTree ITR PREV
*****/

/*****
BSTree ITR REMOVE
*****/

/*****
BSTree ITR GET
*****/
void* BSTreeItrGet(BSTreeItr _it)
{
	if (IS_NULL(_it))
	{
		return NULL;
	}
	return ((Node*)_it)->m_item;
}


/*****
BSTree FOR EACH
*****/
static void InOrderTraverse(Node* _node, void* _context, ActionFunction _action)
{
	if (NULL == _node)
	{
		return;
	}
	InOrderTraverse(_node->m_left, _context, _action);
	_action(_node, _context);
	InOrderTraverse(_node->right, _context, _action);
	
	//TreePrintRecIn(_node->m_left);
	//printf("%d\n", _node->m_data);
	//TreePrintRecIn(_node->m_right);
	return;
}

BSTreeItr BSTreeForEach(const BSTree* _tree, TreeTraversalMode _mode, ActionFunction _action, void* _context)
{
	if(!IS_A_TREE(_tree) || IS_NULL(_mode || IS_NULL(_action)))
	{
		if (NULL != TREE_ROOT(T))
		{
			switch (_traverseMode)
			{
			case BSTREE_TRAVERSAL_INORDER:
				InOrderTraverse(TREE_ROOT(T), _context, _action);
				break;
			}
		}
		return NULL;
	}
	
}

























/*

BSTreeItr BSTreeFindFirst(const BSTree* _tree, PredicateFunction _predicate, void* _context)
BSTreeItr BSTreeItrBegin(const BSTree* _tree)
BSTreeItr BSTreeItrEnd(const BSTree* _tree)
int BSTreeItrEquals(BSTreeItr _a, BSTreeItr _b)
BSTreeItr BSTreeItrNext(BSTreeItr _it)
BSTreeItr BSTreeItrPrev(BSTreeItr _it)
void* BSTreeItrRemove(BSTreeItr _it)
void* BSTreeItrGet(BSTreeItr _it)
BSTreeItr BSTreeForEach(const BSTree* _tree, TreeTraversalMode _mode, ActionFunction _action, void* _context)
*/



























