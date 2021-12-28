#include "global.h"

DWORD CALLBACK ClientThread(LPVOID arg)
{
	ClientInfo* client = (ClientInfo*)arg;
	bool endflag = false;
	while (1)
	{
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