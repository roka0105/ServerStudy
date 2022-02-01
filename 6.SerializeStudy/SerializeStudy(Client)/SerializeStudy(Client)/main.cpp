#include "Player.h"
int main()
{
	int retval = 0;
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)return -1;
	
	SOCKET client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock == INVALID_SOCKET)
		err_quit((char*)"socket");
	SOCKADDR_IN client_addr;
	int addrsize = sizeof(client_addr);
	ZeroMemory(&client_addr, sizeof(SOCKADDR_IN));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr(SERVERIP);
	client_addr.sin_port = htons(SERVERPORT);
	retval = connect(client_sock, (SOCKADDR*)&client_addr,addrsize);
	if (retval == SOCKET_ERROR)
		err_quit((char*)"connect");
	Player player;
	ZeroMemory(&player, sizeof(Player));
	//while (1)
    //{
		RecvPlayerInfo(client_sock, &player);
		cout << "PlayerInfo" << endl;
		player.Print();
	//}
	closesocket(client_sock);
	WSACleanup();
	return 0;
}