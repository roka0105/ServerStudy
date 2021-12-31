#define MAIN
#include "global.h"

int main(int argv, char* argc[])
{
	FileLoad();
	
	srand(time(NULL));
	int retval = 0;
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) return -1;

	SOCKET listensocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listensocket == INVALID_SOCKET)err_quit((char*)"socket()");
	SOCKADDR_IN listenaddr;
	listenaddr.sin_family = AF_INET;
	listenaddr.sin_port = htons(SERVERPORT);
	listenaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listensocket, (SOCKADDR*)&listenaddr, sizeof(listenaddr));
	if (retval == SOCKET_ERROR) err_quit((char*)"bind()");
	retval = listen(listensocket, SOMAXCONN);
	if (retval == SOCKET_ERROR)err_quit((char*)"listen()");
	InitRoom();
    UserLogOut(true);
	
	InitializeCriticalSection(&cs);
	while (1)
	{  
		ClientInfo* client=new ClientInfo();
		int addrsize = sizeof(client->addr);
		client->sock = accept(listensocket, (SOCKADDR*)&client->addr, &addrsize);
		if (client->sock == INVALID_SOCKET)
		{
			err_display((char*)"accept()");
			continue;
		} 
		client->hWaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		client->hEndEvent = CreateEvent(NULL, FALSE, FALSE, NULL); 
		client->hTimercheck = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (client->hWaitEvent == NULL)
		{
			RemoveClient(client);
			break;
		}
		AddClient(client);
		
		hThread = CreateThread(0, NULL, ClientThread, client, 0, NULL);
		if (hThread == NULL)
			RemoveClient(client);
		else CloseHandle(hThread);
	}
	DeleteCriticalSection(&cs);
	closesocket(listensocket);
	WSACleanup();
	FileSave();
	return 0;
}

