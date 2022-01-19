#include "Socket.h"
Socket::Socket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
}
Socket::Socket(SOCKET _sock, SOCKADDR_IN _addr)
{
	sock = _sock;
	memcpy(&addr, &_addr, sizeof(SOCKADDR_IN));
}
Socket::Socket(const Socket& ref)
{
	sock = ref.sock;
	memcpy(&addr, &ref.addr, sizeof(SOCKADDR_IN));
}
SOCKADDR_IN Socket::GetAddr()
{
	return addr;
}
void Socket::err_quit(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpmsgbuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(1);
}
void Socket::err_display(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpmsgbuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCTSTR)msg, MB_ICONERROR);
	//printf("[%s]%s", (LPCTSTR)msg, lpmsgbuf);
	cout << '[' << msg << ']' << lpmsgbuf;
	LocalFree(lpmsgbuf);
}
Socket::~Socket()
{
	closesocket(sock);
}