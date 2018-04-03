#include "ClientCPP.h"
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<csignal>
using namespace std;

//int main()
//{
//	netcpp::Client client;
//	client.Connect(8888, "127.0.0.1");
//	client.Run();
//	sleep(5);
//	return 0;


static bool keepRunning = true;

void intHandler(int dummy) 
{
    keepRunning = 0;
}
	
	

	
	
	
	
int main(int _argc, char* _argv[])
{	
	const int CLIENT_NUM = 100;
	int randClient;
	size_t index;
	netcpp::Client smartClientArr[CLIENT_NUM];
	size_t numOfConnected = 0;
	char ip[128];
	int port;
	size_t numToDisconnect = 0;
	size_t numToRun = 0;
	size_t numToConnect = 0;
	signal(SIGINT, intHandler);
//	
	srand(time(NULL));
	
	/*******
	Check input arguments (IP) and Init
	*******/
	if(1 == _argc)
	{
		strcpy(ip, "127.0.0.1");
		port = 8888;
	}
	else if(3 == _argc)
	{
		strcpy(ip, _argv[1]);
		port = atoi(_argv[2]);
	}
	else
	{
		std::cout << "Invalid num of arguments to main() " << std::endl;
		abort();
	}
	/*******
	Create and Connect all Clients and Do initial Run
	*******/
	for (index = 0; index < CLIENT_NUM; ++index)
	{
		smartClientArr[index].Connect(port, ip);
		std::cout << "Client " << index << "connected" << endl;
		++numOfConnected;
		smartClientArr[index].Run();
	}
	/*******
	Do SmartClient Loop
	*******/
	while (keepRunning)
	{
		/*******
		10% of connected clients should disconnect
		*******/
		numToDisconnect = (numOfConnected * 10) / 100;
		for(index = 0; index < numToDisconnect; ++index)
		{
			randClient = rand() % CLIENT_NUM;
			while (!smartClientArr[randClient].IsConnected())
			{
				randClient = rand() % CLIENT_NUM;
			}
			smartClientArr[randClient].Disconnect();
			--numOfConnected;
			std::cout << "Client " << randClient << "disconnected" << endl;
		}
		/*******
		30% of connected clients should run
		*******/
		numToRun = (numOfConnected * 30) / 100;
		for(index = 0; index < numToRun; ++index)
		{
			randClient = rand() % CLIENT_NUM;
			while (!smartClientArr[randClient].IsConnected())
			{
				randClient = rand() % CLIENT_NUM;
			}
			smartClientArr[randClient].Run();
			std::cout << "Client " << randClient << "ran" << endl;
		}
		/*******
		30% of disconnected clients should connect
		*******/
		numToConnect = (30 *(CLIENT_NUM - numOfConnected)) / 100;
		for(index = 0; index < numToConnect; ++index)
		{
			randClient = rand() % CLIENT_NUM;
			while (smartClientArr[randClient].IsConnected())
			{
				randClient = rand() % CLIENT_NUM;
			}
			smartClientArr[randClient].Connect(port, ip);
			++numOfConnected;
			std::cout << "Client " << randClient << "connected" << endl;
		}
	}
//	/*******
//	Destroy all Clients after receiving SIGINT
//	*******/
//	for (index = 0; index < CLIENT_NUM; ++index)
//	{
//		ClientDestroy(smartClientArr[index].m_client);
//		printf("Destroyed Client %lu\n", (long unsigned int)index);
//	}

	return 0;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}