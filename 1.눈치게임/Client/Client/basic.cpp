#include "global.h"

void err_quit(char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpmsgbuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)&lpmsgbuf, msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(-1);
}
void err_display(char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpmsgbuf, 0, NULL);
	printf("[%s]%s", msg, lpmsgbuf);
	LocalFree(lpmsgbuf);
}
int recvn(SOCKET s, char* buf, int len, bool flag)
{
	char* ptr = buf;
	int received;
	int left = len;
	while (left > 0)
	{
		received = recv(s, ptr, left, flag);
		if (received == SOCKET_ERROR)return SOCKET_ERROR;
		else if (received == 0)return 0;

		ptr += received;
		left -= received;
	}
	return (len - left);
}