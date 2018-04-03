#include "Mutex.h"
#include<iostream>
#include<unistd.h>

typedef struct MyStruct
{
	advcpp::Mutex* 	m_mutex;
	int*	m_xPtr;
}MyStruct;

void* ThreadFunc1(void* mystruct)
{
	MyStruct* msPtr = (MyStruct*)mystruct;
	msPtr->m_mutex->Lock();
	std::cout << "(before increment)x = " << *msPtr->m_xPtr << std::endl;
	++(*msPtr->m_xPtr);
	std::cout << "(after increment)x = " << *msPtr->m_xPtr << std::endl;
	--(*msPtr->m_xPtr);
	std::cout << "(after decrement)x = " << *msPtr->m_xPtr << std::endl;
	msPtr->m_mutex->Unlock();
	return 0;
}



int main()
{
	const size_t numOfThreads = 20;
	int x = 17;
	pthread_t threads[numOfThreads];
	MyStruct mystruct;
	advcpp::Mutex mutex;
	mystruct.m_mutex = &mutex;
	mystruct.m_xPtr = &x;
	
	for (size_t index = 0; index < numOfThreads; ++index)
	{
		pthread_create(&threads[index], 0, ThreadFunc1, &mystruct);	
	}
	
//	sleep(0.2);
	
	for (size_t index = 0; index < numOfThreads; ++index)
	{
		pthread_join(threads[index], 0);	
	}
	
	return 0;
}