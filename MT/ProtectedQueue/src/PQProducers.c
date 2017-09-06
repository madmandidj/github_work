
#include "../inc/PQProducers.h"
#include "../inc/PCPQconfig.h"
#include "../inc/ProtectedQueue.h"

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>



struct Producers
{
	ProQueue* 	m_P2C;
	ProQueue* 	m_C2P;
	void**		m_msgArr;
/*	void**		m_rcvMsg;*/
/*	void*		m_rcvMsg;*/
	pthread_t* 	m_threadIDs;
	size_t 		m_numOfProds;
	size_t		m_numOfMsgs;
	size_t		m_speed;
	size_t 		m_verbosity;
	ReadMsg		m_readFunc;
};






/*Producers* ProducersCreate(size_t _numOfProds, size_t _numOfMsgs, void** _msgArr, ProQueue* _P2C, ProQueue* _C2P)*/
Producers* ProducersCreate(Params* _params, void** _msgArr, ProQueue* _P2C, ProQueue* _C2P, ReadMsg _readFunc)
{
	Producers* prods;	
	
	if (NULL == _params || NULL == _msgArr || NULL == _P2C || NULL == _C2P)
	{
		return NULL;
	}
	
	prods = malloc(sizeof(Producers));
	if (NULL == prods)
	{
		return NULL;
	}
	
	prods->m_numOfProds = GetNumOfProds(_params);
	prods->m_numOfMsgs = GetNumOfMsgs(_params);
	prods->m_speed = GetSpeed(_params);
	prods->m_verbosity = GetVerbosity(_params);
	
	prods->m_threadIDs = malloc(prods->m_numOfProds * sizeof(pthread_t));
	if (NULL == prods->m_threadIDs)
	{
		free(prods);
		return NULL;
	}
	
/*	prods->m_rcvMsg = malloc(sizeof(void*));*/
/*	if (NULL == prods->m_rcvMsg)*/
/*	{*/
/*		free(prods->m_threadIDs);*/
/*		free(prods);*/
/*		return NULL;*/
/*	}*/
	
	prods->m_P2C = _P2C;
	prods->m_C2P = _C2P;
	prods->m_msgArr = _msgArr;
/*	prods->m_rcvMsg = NULL;*/
	prods->m_readFunc = _readFunc;
	
	return prods;
}





void ProducersDelete(Producers* _prods)
{
	if (NULL == _prods)
	{
		return;
	}
	
	free(_prods->m_threadIDs);
	free(_prods);
	
	return;
}







static void* ProducersRoutine(void* _prods)
{	
	size_t index;
	void* rcvMsg; 

	
	if (NULL == _prods)
	{
		return NULL;
	}

	for(index = 0; index < ((Producers*)_prods)->m_numOfMsgs; ++index)
	{
		/*
		ProQueueInsert(((Producers*)_prods)->m_P2C, ((Producers*)_prods)->m_msgArr[index]);
		*/
		ProQueueInsert(((Producers*)_prods)->m_P2C, ((Producers*)_prods)->m_msgArr + index);
		
		if (((Producers*)_prods)->m_verbosity)
		{
			((Producers*)_prods)->m_readFunc(((Producers*)_prods)->m_msgArr + index, "Producer", "Inserted");
		}
		
		 ProQueueRemove(((Producers*)_prods)->m_C2P, &rcvMsg); 
		
/*		ProQueueRemove(((Producers*)_prods)->m_C2P, &((Producers*)_prods)->m_rcvMsg);*/
/*		ProQueueRemove(((Producers*)_prods)->m_C2P, ((Producers*)_prods)->m_rcvMsg);*/
		
		if (((Producers*)_prods)->m_verbosity)
		{
			((Producers*)_prods)->m_readFunc(rcvMsg, "Producer", "Removed");
/*			((Producers*)_prods)->m_readFunc(((Producers*)_prods)->m_rcvMsg, "Producer", "Removed");*/
/*			((Producers*)_prods)->m_readFunc(*((Producers*)_prods)->m_rcvMsg, "Producer", "Removed");*/
		}
		
		usleep(((Producers*)_prods)->m_speed);
	}

	pthread_exit(NULL);
	
	return NULL;
}







int ProducersRun(Producers* _prods)
{
	size_t index;
	
	if (NULL == _prods)
	{
		return 1;
	}
	
	for (index = 0; index < _prods->m_numOfProds; ++index)
	{
		if (pthread_create(_prods->m_threadIDs + index, NULL, ProducersRoutine, _prods))
		{
			return 1;
		}
	}
	
	return 0;
}







void ProducersJoin(Producers* _prods)
{
	size_t index;
	
	if (NULL == _prods)
	{
		return;
	}
	
	for (index = 0; index < _prods->m_numOfProds; ++index)
	{
		pthread_join(_prods->m_threadIDs[index], NULL);
	}
	
	return;
}







