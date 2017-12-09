#include "Client_t.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFFER_LEN 4096
/*************************
**************************
Client_t struct definition
**************************
**************************/
struct Client_t
{
	int m_sock;
};
/************************************
*************************************
Static functions forward declarations
*************************************
************************************/
int ClientInit(Client_t* _client, int _port, char* _ip);
/*static int ClientConnect(Client_t* _client, int _port, char* _ip);*/
/************************************
*************************************
Server_t API function implementations
*************************************
************************************/
Client_t* ClientCreate(int _port, char* _ip)
{
	/*TODO: check valid params */
	Client_t* client = malloc(sizeof(Client_t));
	if (NULL == client)
	{
		return NULL;
	}
	
	if (!ClientInit(client, _port, _ip))
	{
		return NULL;
	}
	
	return client;
}

void ClientDestroy(Client_t* _client)
{
	close(_client->m_sock);
	free(_client);
}

void ClientRun(Client_t* _client)
{
	char data[] = "Maftiya\n\0";
	char buffer[strlen(data) + 1];
	write(_client->m_sock, data, strlen(data) + 1);
	printf("Client sent %s\n", data);
/*	sleep(2);*/
	read(_client->m_sock, buffer, BUFFER_LEN);
	printf("Client received %s\n", buffer);
}

int ClientConnect(Client_t* _client, int _port, char* _ip)
{
	struct sockaddr_in sin;
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(_ip);
	sin.sin_port = htons(_port);
	if (connect(_client->m_sock, (struct sockaddr*) &sin, sizeof(sin)) < 0)
	{
		free(_client);
		return 0;
	}
	return 1;
}

void ClientDisconnect(Client_t* _client)
{
	close(_client->m_sock);
}

/***************************************
***************************************
Server_t Static function implementations 
***************************************
***************************************/
int ClientInit(Client_t* _client, int _port, char* _ip)
{
	_client->m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > _client->m_sock)
	{
		free(_client);
		return 0;
	}
	
	
	return ClientConnect(_client, _port, _ip);
}




























