#include "global.h"
void err_quit(char* msg)
{
	LPVOID lpMsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgbuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgbuf, (LPCWSTR)msg, MB_OK);
	LocalFree(lpMsgbuf);
	exit(-1);
}
void err_display(char* msg)
{
	LPVOID lpMsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgbuf, 0, NULL);
	printf("[%s]%s", msg, lpMsgbuf);
	LocalFree(lpMsgbuf);
}
int recvn(SOCKET sock, char* buf, int len, bool flag)
{
	char* ptr = buf;
	int received = 0;
	int left = len;
	while (left > 0)
	{
		received = recv(sock, ptr, left, flag);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0) return 0;
		ptr += received;
		left -= received;
	}
	return len - left;
}
void SendPlayerInfo(SOCKET sock, const Player* player)
{
	int retval = 0;
	OutPutMemoryStream oStream;
	player->Write(oStream);
	retval = send(sock, oStream.GetStreamPtr(), oStream.GetLength(), 0);
	if (retval == SOCKET_ERROR)
		err_display((char*)"Player_send");
}
const int MAX_PacketSize = 1470;
void RecvPlayerInfo(SOCKET sock, Player* player)
{
	int retval = 0;
	char* buf = new char[sizeof(Player)];
	ZeroMemory(buf, sizeof(Player));
	retval = recv(sock, buf, sizeof(Player), 0);//recvn(sock, buf, sizeof(Player), 0);
	if (retval == SOCKET_ERROR)
		err_display((char*)"Player_recv");
	if (retval > 0)
	{
		InPutMemoryStream iStream(buf, retval);
		player->Read(iStream);
	}
	else delete buf;

}