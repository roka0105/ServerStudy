#include "global.h"
DWORD CALLBACK TimerThread(LPVOID arg)
{
	ClientInfo* client = (ClientInfo*)arg;
	float now = 0;
	while (1)
	{
		WaitForSingleObject(client->room->game->hTimerStartEvent, INFINITE);
		if (client->state == STATE::MAINMENU)
		{
			break;
		}
		while (1)
		{
			now = (clock() - client->room->game->start_time) / CLOCKS_PER_SEC;
			printf("%fÃÊ\n", now);
			if (now >= LIMITTIME)
			{
				for (int i = 0; i < client->room->attend_count; ++i)
				{
					SetEvent(client->room->client[i]->hTimercheck);
					//SetEvent(client->room->client[i]->hWaitEvent);
				}
				client->room->game->Next = true;
				break;
			}

		}
	}
	return 0;
}
DWORD CALLBACK ClientThread(LPVOID arg)
{
	ClientInfo* client = (ClientInfo*)arg;
	bool endflag = false;
	while (1)
	{
		ZeroMemory(client->recvbuf, MAXBUF);
		ZeroMemory(client->sendbuf, MAXBUF);
		switch (client->state)
		{
		case STATE::MAINMENU:
			MenuProcess(client);
			break;
		case STATE::LOGIN:
			LoginProcess(client);
			break;
		case STATE::JOIN:
			JoinProcess(client);
			break;
		case STATE::ROOMLIST:
			RoomListProcess(client);
			break;
		case STATE::ROOM:
			RoomProcess(client);
			break;
		case STATE::GAMESTART:
			GameStartProcess(client);
			break;
		case STATE::END:
			EndProcess(client);
			break;
		case STATE::EXIT:
			UserLogOut(false, client);
			ExitProcess(client);
			RemoveClient(client);
			endflag = true;
			break;
		}
		if (endflag)
			break;
	}
	return 0;
}