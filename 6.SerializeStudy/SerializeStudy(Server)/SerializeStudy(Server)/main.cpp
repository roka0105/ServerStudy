#include "Player.h"
int main()
{
	int retval = 0;
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)return -1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit((char*)"socket");
	SOCKADDR_IN listen_addr;
	ZeroMemory(&listen_addr, sizeof(SOCKADDR_IN));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&listen_addr, sizeof(listen_addr));
	if (retval == SOCKET_ERROR)
		err_quit((char*)"bind");
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit((char*)"listen");
	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrsize = sizeof(client_addr);

	client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrsize);
	if (client_sock == INVALID_SOCKET)
		err_quit((char*)"accept");
	cout << "[TCP서버 접속] IP:" << inet_ntoa(client_addr.sin_addr) << "PORT:" << ntohs(client_addr.sin_port);
	Player player(10,20,160,"최예람");
	//while (1)
	//{
		SendPlayerInfo(client_sock, &player);

	//}
	cout << "[TCP서버 종료] IP:" << inet_ntoa(client_addr.sin_addr) << "PORT:" << ntohs(client_addr.sin_port);
	closesocket(client_sock);
	closesocket(listen_sock);
	WSACleanup();
	return 0;
}