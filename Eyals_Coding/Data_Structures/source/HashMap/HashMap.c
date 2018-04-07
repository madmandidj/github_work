#include "../../inc/HashMap.h"
#include "../../inc/List.h"
#include "../../inc/ADTErr.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct KeyAndVal_t
{
	Key_t*	m_key;
	Value_t*	m_value;
}KeyAndVal_t;

struct HashMap
{
	List** 			m_buckets;
	HashFunc 		m_hashFunc;
	EqualityFunc	m_equalFunc;
	size_t			m_numOfElements;
	size_t			m_userCapacity;
	size_t			m_actualCapacity;
	#ifndef NDEBUG
	MapStats* 		m_mapStats;
	#endif /* NDEBUG */
};

static size_t CalcHashSize(size_t _num)
{
    int isPrime = 0;
    size_t i;
    _num = _num < 10 ? 10 : _num;
/*    _num += 1;*/
    _num *= 11;
    _num /= 10;

    while (!isPrime)
    {
        isPrime = 1;   
        for (i = 2; i < sqrt(_num) ; ++i)
        {
            if (_num % i == 0)
            {
				isPrime = 0;
                break;
			}
		}
		if (isPrime == 0)
		{
			++_num;
		}
		else
		{
			break;
		}
    }
    return _num;
}

HashMap* HashMapCreate(size_t _capacity, HashFunc _hashFunc, EqualityFunc _equalFunc)
{
	HashMap* hashMap;
/*	size_t index;*/
	if (0 == _capacity || !_hashFunc || !_equalFunc)
	{
		return NULL;
	}
	
	if (!(hashMap = malloc(sizeof(HashMap))))
	{
		return NULL;
	}
	hashMap->m_hashFunc = _hashFunc;
	hashMap->m_equalFunc = _equalFunc;
	hashMap->m_numOfElements = 0;
	hashMap->m_userCapacity = _capacity;
	hashMap->m_actualCapacity = CalcHashSize(hashMap->m_userCapacity);
	if (!(hashMap->m_buckets = calloc(hashMap->m_actualCapacity, sizeof(List*))))
	{
		free(hashMap);
		return NULL;
	}
	#ifndef NDEBUG
	if (!(hashMap->m_mapStats = malloc(sizeof(MapStats))))
	{
		free(hashMap->m_buckets);
		free(hashMap);
		return NULL;
	}
	hashMap->m_mapStats->m_pairs = 0;
	hashMap->m_mapStats->m_collisions = 0;
	hashMap->m_mapStats->m_buckets = hashMap->m_actualCapacity;
	hashMap->m_mapStats->m_chains = 0;
	hashMap->m_mapStats->m_maxChainLength = 0;
	hashMap->m_mapStats->m_averageChainLength = 0;
	#endif /* NDEBUG */
	return hashMap;
}

void HashMapDestroy(HashMap* _hashMap, KeyDestroyFunc _keyDestroyFunc, ValueDestroyFunc _valDestroyFunc)
{
	size_t index;
	ListItr itr;
/*	ListItr listEnd;*/
/*	size_t numOfActions = 0;*/
	KeyAndVal_t* keyAndValPair;
	Key_t* curKey;
	Value_t* curVal;
	if (!_hashMap)
	{
		return;
	}
	
	for (index = 0; index < _hashMap->m_actualCapacity; ++index)
	{
		while (_hashMap->m_buckets[index])
		{
			itr = ListItrFirst(_hashMap->m_buckets[index]);
			keyAndValPair = (KeyAndVal_t*)ListItrGet(itr);
			HashMapRemove(_hashMap, keyAndValPair->m_key, &curKey, &curVal);
			if (_keyDestroyFunc)
			{
				_keyDestroyFunc(curKey);
			}
			if (_valDestroyFunc)
			{
				_valDestroyFunc(curVal);
			}
		}
	}
	#ifndef NDEBUG
	free(_hashMap->m_mapStats);
	#endif /* NDEBUG */
	free(_hashMap->m_buckets);
	free(_hashMap);
	return;
}


ADTErr HashMapInsert(HashMap* _hashMap, const Key_t* _key, const Value_t* _value)
{
/*	size_t index;*/
	size_t insertionIndex;
	ListItr itr;
	ListItr listEnd;
	KeyAndVal_t* newKeyValPair = NULL;
	KeyAndVal_t* returnedKeyValPair = NULL;
	if (!_hashMap || !_key || !_value)
	{
		return ERR_INVARG;
	}
	
	insertionIndex = _hashMap->m_hashFunc(_key);
	insertionIndex %= _hashMap->m_actualCapacity;
	if (_hashMap->m_buckets[insertionIndex])
	{
		listEnd = ListItrEnd(_hashMap->m_buckets[insertionIndex]);
		itr = ListItrFirst(_hashMap->m_buckets[insertionIndex]);
		while (itr != listEnd)
		{		
			returnedKeyValPair = (KeyAndVal_t*) ListItrGet(itr);
			if (returnedKeyValPair && _hashMap->m_equalFunc(_key, returnedKeyValPair->m_key))
			{
				return ERR_MAP_KEY_FOUND;
			}
			itr = ListItrNext(itr);
		}
		#ifndef NDEBUG
		if (1 <= ListItemsNum(_hashMap->m_buckets[insertionIndex]))
		{
			++_hashMap->m_mapStats->m_collisions;
		}
		#endif /* NDEBUG */
	}
	else
	{		
		_hashMap->m_buckets[insertionIndex] = ListCreate();
		#ifndef NDEBUG
		++_hashMap->m_mapStats->m_chains;
		#endif /* NDEBUG */
	}
	if (!(newKeyValPair = malloc(sizeof(KeyAndVal_t))))
	{
		return ERR_NOMEM;
	}
	newKeyValPair->m_key = (Key_t*)_key;
	newKeyValPair->m_value = (Value_t*)_value;
	ListPushTail(_hashMap->m_buckets[insertionIndex], (void*)newKeyValPair);
	++_hashMap->m_numOfElements;
	#ifndef NDEBUG
	++_hashMap->m_mapStats->m_pairs;
	#endif /* NDEBUG */
	
	return ERR_OK;
}

ADTErr HashMapRemove(HashMap* _hashMap, const Key_t* _searchKey, Key_t** _removedKey, Value_t** _removedValue)
{
/*	size_t index;*/
	size_t insertionIndex;
	ListItr itr;
	ListItr listEnd;
	KeyAndVal_t* foundKeyValPair = NULL;
	KeyAndVal_t* dummyPair = NULL;
	if (!_hashMap || !_searchKey || !_removedKey || !_removedValue)
	{
		return ERR_INVARG;
	}
	
	insertionIndex = _hashMap->m_hashFunc(_searchKey);
	insertionIndex %= _hashMap->m_actualCapacity;
	if (_hashMap->m_buckets[insertionIndex])
	{
		listEnd = ListItrEnd(_hashMap->m_buckets[insertionIndex]);
		itr = ListItrFirst(_hashMap->m_buckets[insertionIndex]);
		while (itr != listEnd)
		{		
			foundKeyValPair = (KeyAndVal_t*) ListItrGet(itr);
			if (_hashMap->m_equalFunc(_searchKey, foundKeyValPair->m_key))
			{
				*_removedKey = foundKeyValPair->m_key;
				*_removedValue = foundKeyValPair->m_value;				
				itr = ListItrRemove(_hashMap->m_buckets[insertionIndex], itr, (void**)&dummyPair);
				free(dummyPair);
				if (0 == ListItemsNum(_hashMap->m_buckets[insertionIndex]))
				{
					ListDestroy(_hashMap->m_buckets[insertionIndex], NULL);
					_hashMap->m_buckets[insertionIndex] = NULL;
					#ifndef NDEBUG
					--_hashMap->m_mapStats->m_chains;
					#endif /* NDEBUG */
				}
				#ifndef NDEBUG
				else if (1 <= ListItemsNum(_hashMap->m_buckets[insertionIndex]))
				{
					--_hashMap->m_mapStats->m_collisions;
				}
				--_hashMap->m_mapStats->m_pairs;
				#endif /* NDEBUG */
				--_hashMap->m_numOfElements;
				return ERR_OK;
			}
			itr = ListItrNext(itr);
		}
	}
	*_removedKey = NULL;
	*_removedValue = NULL;
	return ERR_MAP_KEY_NOT_FOUND;	
}
/*
ADTErr HashMapRehash(HashMap** _hashMap, size_t _newCapacity)
{
	HashMap* hashMap;
	ListItr itr;
	KeyAndVal_t* curKeyAndVal;
	Key_t* curKey;
	Value_t* curVal;
	size_t index;
	
	if (!_hashMap || !*_hashMap || 0 == _newCapacity)
	{
		return ERR_INVARG;
	}
	
	if (!(hashMap = HashMapCreate(_newCapacity, (*_hashMap)->m_hashFunc, (*_hashMap)->m_equalFunc)))
	{
		return ERR_NOMEM;
	}
	for (index = 0; index < (*_hashMap)->m_actualCapacity; ++index)
	{
		while ((*_hashMap)->m_buckets[index])
		{
			itr = ListItrFirst((*_hashMap)->m_buckets[index]);
			curKeyAndVal = ListItrGet(itr);
			HashMapRemove((*_hashMap), curKeyAndVal->m_key, &curKey, &curVal);
			HashMapInsert(hashMap, curKey, curVal);
		}
	}
	HashMapDestroy(*_hashMap, NULL, NULL);
	*_hashMap = hashMap;
	return ERR_OK;
}
*/

ADTErr HashMapRehash(HashMap* _hashMap, size_t _newCapacity)
{
	List** newBuckets;
	size_t newActualCapacity;
	size_t numOfItems;
	size_t curItemNum = 0;
	
/*	HashMap* hashMap;*/
/*	ListItr itr;*/
	KeyAndVal_t* curKeyAndVal;
	Key_t** keyArr;
	Value_t** valArr;
	size_t index;
	
	if (!_hashMap || 0 == _newCapacity)
	{
		return ERR_INVARG;
	}
	
	newActualCapacity = CalcHashSize(_newCapacity);
	numOfItems = _hashMap->m_numOfElements;
	if (!(newBuckets = calloc(newActualCapacity, sizeof(List*))))
	{
		return ERR_NOMEM;
	}
	if (!(keyArr = malloc(numOfItems * sizeof(Key_t*))))
	{
		free(newBuckets);
		return ERR_NOMEM;
	}
	if (!(valArr = malloc(numOfItems * sizeof(Key_t*))))
	{
		free(keyArr);
		free(newBuckets);
		return ERR_NOMEM;
	}
	
	for (index = 0; index < _hashMap->m_actualCapacity; ++index)
	{
		if (_hashMap->m_buckets[index])
		{
			while(0 != ListItemsNum(_hashMap->m_buckets[index]))
			{
				ListPopHead(_hashMap->m_buckets[index], (void**)&curKeyAndVal);
				keyArr[curItemNum] = curKeyAndVal->m_key;
				valArr[curItemNum] = curKeyAndVal->m_value;
				free(curKeyAndVal);
				++curItemNum;
			}
			ListDestroy(_hashMap->m_buckets[index], NULL);
		}
	}
	free(_hashMap->m_buckets);
	_hashMap->m_buckets = newBuckets;
	_hashMap->m_numOfElements = 0;
	_hashMap->m_userCapacity = _newCapacity;
	_hashMap->m_actualCapacity = newActualCapacity;
	#ifndef NDEBUG
	_hashMap->m_mapStats->m_pairs = 0;
	_hashMap->m_mapStats->m_collisions = 0;
	_hashMap->m_mapStats->m_buckets = _hashMap->m_actualCapacity;
	_hashMap->m_mapStats->m_chains = 0;
	_hashMap->m_mapStats->m_maxChainLength = 0;
	_hashMap->m_mapStats->m_averageChainLength = 0;
	#endif /* NDEBUG */
	for (index = 0; index < numOfItems; ++index)
	{
		HashMapInsert(_hashMap, keyArr[index], valArr[index]);
	}
	free(keyArr);
	free(valArr);
	return ERR_OK;
}

ADTErr HashMapFind(HashMap* _hashMap, const Key_t* _searchKey, Value_t** _foundValue)
{
/*	size_t index;*/
	size_t insertionIndex;
	ListItr itr;
	ListItr listEnd;
	KeyAndVal_t* foundKeyValPair = NULL;
	if (!_hashMap || !_searchKey || !_foundValue)
	{
		return ERR_INVARG;
	}
	
	insertionIndex = _hashMap->m_hashFunc(_searchKey);
	insertionIndex %= _hashMap->m_actualCapacity;
	if (_hashMap->m_buckets[insertionIndex])
	{
		listEnd = ListItrEnd(_hashMap->m_buckets[insertionIndex]);
		itr = ListItrFirst(_hashMap->m_buckets[insertionIndex]);
		while (itr != listEnd)
		{		
			foundKeyValPair = (KeyAndVal_t*) ListItrGet(itr);
			if (_hashMap->m_equalFunc(_searchKey, foundKeyValPair->m_key))
			{
				*_foundValue = foundKeyValPair->m_value;				
				return ERR_OK;
			}
			itr = ListItrNext(itr);
		}
	}
	*_foundValue = NULL;
	return ERR_MAP_KEY_NOT_FOUND;
}


size_t HashMapSize(HashMap* _hashMap)
{
	return _hashMap ? _hashMap->m_numOfElements : 0; 
}



size_t HashMapForEach(HashMap* _hashMap, MapElementActionFunc _actionFunc, void* _context)
{
	size_t index;
	ListItr itr;
	ListItr listEnd;
	size_t numOfActions = 0;
	KeyAndVal_t* keyAndValPair;
	if (!_hashMap || !_actionFunc || 0 == _hashMap->m_numOfElements)
	{
		return 0;
	}
	
	for (index = 0; index < _hashMap->m_actualCapacity; ++index)
	{
		if (_hashMap->m_buckets[index])
		{
			listEnd = ListItrEnd(_hashMap->m_buckets[index]);
			itr = ListItrFirst(_hashMap->m_buckets[index]);
			while (itr != listEnd)
			{
				keyAndValPair = (KeyAndVal_t*)ListItrGet(itr);
				if (!(_actionFunc(keyAndValPair->m_key, keyAndValPair->m_value, _context)))
				{
					return numOfActions;
				}
				++numOfActions;
				itr = ListItrNext(itr);
			}
		}
	}
	return numOfActions;
}


#ifndef NDEBUG
MapStats HashMapGetStatistics(const HashMap* _hashMap)
{
	MapStats tempMapStats = {0};
	size_t index;
	size_t curChainLength = 0;
	size_t maxChainLength = 0;
	size_t chainLengthSum = 0;
	float averageChainLengthFloat;
	float chainsFloat;
	if (!_hashMap)
	{
		return tempMapStats;
	}
	
	_hashMap->m_mapStats->m_buckets = _hashMap->m_actualCapacity;
	for (index = 0; index < _hashMap->m_actualCapacity; ++index)
	{
		if (_hashMap->m_buckets[index])
		{
			curChainLength = ListItemsNum(_hashMap->m_buckets[index]);
			maxChainLength = curChainLength > maxChainLength ? curChainLength : maxChainLength;
			chainLengthSum += curChainLength;
		}
	}
	averageChainLengthFloat = (float)chainLengthSum;
	chainsFloat = (float)(_hashMap->m_mapStats->m_chains);
	_hashMap->m_mapStats->m_averageChainLength = _hashMap->m_mapStats->m_chains ? averageChainLengthFloat / chainsFloat : 0;
	_hashMap->m_mapStats->m_maxChainLength = maxChainLength;
	return *(_hashMap->m_mapStats);
}
#endif /* NDEBUG */


















