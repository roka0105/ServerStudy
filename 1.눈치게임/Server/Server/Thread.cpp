#include "global.h"
DWORD CALLBACK TimerThread(LPVOID arg)
{
	ClientInfo* client = (ClientInfo*)arg;
	float now = 0;
	while (1)
	{
		WaitForMultipleObjects(LIMITNUM, client->room->game->hTimerStartEvent, FALSE, INFINITE);
		while (1)
		{
			now = (clock() - client->room->game->start_time) / CLOCKS_PER_SEC;
			printf("%f??\n", now);
			if (now >= LIMITTIME)
			{
				client->room->game->Next = true;
				if (client->room->game->loseresult)
				{
					for (int i = 0; i < client->room->attend_count; ++i)
					{
						if (client->room->client[i]->protocol == PROTOCOL::SAMETIME)
						{
							for (int j = 0; j < LIMITNUM; ++j)
							{
								SetEvent(client->room->client[j]->hTimercheck);
							}
							break;
						}
					}
					return 0;
				}
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