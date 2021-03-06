#include "../inc/ProtectedQueue.h"
#include <fcntl.h>			/* O_CREAT */
#include <semaphore.h>		/* sem_t, sem_open */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>





struct ProQueue
{
    void** 				m_vec;         	/*vec to work on*/
    size_t 				m_size;        	/*size of vector*/
    size_t 				m_head;        	/* index of head in m_vec*/
    size_t 				m_tail;        	/* index of tail in m_vec*/
    size_t 				m_nItems;       /* number of items in vector*/
    pthread_mutex_t 	m_qMutex;
    pthread_cond_t		m_cond;
};






ProQueue* ProQueueCreate(size_t _size)
{
	size_t initHead = 0;
	size_t initTail = 0;
	size_t initItems = 0;
	ProQueue* queue;
	
	if(0 == _size)
	{
		return NULL;
	}
	
	queue = malloc(sizeof(ProQueue));
	if(NULL == queue)
	{
		return NULL;
	}
	else
	{
		queue->m_vec = malloc(_size * sizeof(void*));
		if(NULL == queue->m_vec)
		{
			free(queue);
			return NULL;
		}
		
		queue->m_size = _size;
		queue->m_head = initHead;
		queue->m_tail = initTail;
		queue->m_nItems = initItems;
		
		if (pthread_mutex_init(&queue->m_qMutex, NULL))
		{
			free(queue->m_vec);
			free(queue);
			return NULL;
		}
		
		if (pthread_cond_init(&queue->m_cond, NULL))
		{
			pthread_mutex_destroy(&queue->m_qMutex);
			free(queue->m_vec);
			free(queue);
			return NULL;
		}
	}
	
	return queue;
}







void ProQueueDestroy(ProQueue* _queue)
{	
	if(NULL != _queue)
	{
		if(NULL != _queue->m_vec)
		{
			pthread_cond_destroy(&_queue->m_cond);
			pthread_mutex_destroy(&_queue->m_qMutex);
			free(_queue->m_vec);
		}
		free(_queue);
	}
	
	return;
}









int  ProQueueInsert(ProQueue* _queue, void* _item)
{
	if(NULL == _queue || NULL == _queue->m_vec)
	{
		return 0;
	}

	pthread_mutex_lock(&_queue->m_qMutex);
	while(_queue->m_nItems == _queue->m_size)
	{
		pthread_cond_wait(&_queue->m_cond, &_queue->m_qMutex);
	}
	
	_queue->m_vec[_queue->m_head] = _item;
	++(_queue->m_nItems);
	++(_queue->m_head);
	_queue->m_head %= _queue->m_size;
	
	pthread_cond_signal(&_queue->m_cond);
	pthread_mutex_unlock(&_queue->m_qMutex);
	
	return 1;
}









int  ProQueueRemove(ProQueue* _queue, void** _item)
{
	if(NULL == _queue || NULL == _queue->m_vec)
	{
		return 0;
	}
	
	pthread_mutex_lock(&_queue->m_qMutex);
	
	while(0 == _queue->m_nItems)
	{
		pthread_cond_wait(&_queue->m_cond, &_queue->m_qMutex);
	}
	
	
	if(NULL != _item)
	{
		*_item = _queue->m_vec[_queue->m_tail];
	}
	++(_queue->m_tail);
	_queue->m_tail %= _queue->m_size;
	--(_queue->m_nItems);
	
	pthread_cond_signal(&_queue->m_cond);
	pthread_mutex_unlock(&_queue->m_qMutex);

	return 1;
}









int ProQueueIsEmpty(ProQueue* _queue)
{
	int result;
	if(NULL == _queue || NULL == _queue->m_vec)
	{
		result = 1;
	}
	else if(0 == _queue->m_nItems)
	{
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}






