#include "NetworkSocket.h"
NetworkSocket::NetworkSocket()
{
}
NetworkSocket::~NetworkSocket()
{
}
void NetworkSocket::Send(const char* databuf, int size)
{
	int retval = 0;
	retval = send(sock, databuf, size, 0);
	if (retval == SOCKET_ERROR)
		this->err_display("send()");
}
bool  NetworkSocket::Recv(char* databuf, int& size)
{
	int retval = 0;
	int _size = 0;
	retval = recvn((char*)&_size, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
		return false;
	else if (retval == 0)return false;
	char temp[MAXBUF];
	ZeroMemory(temp, MAXBUF);
	retval = recvn(temp, _size, 0);
	if (retval == SOCKET_ERROR)
		return false;
	else if (retval == 0)return false;
	size = _size;
	memcpy(databuf, temp, size);
	return true;
}
void NetworkSocket::err_quit(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpmsgbuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(1);
}
void NetworkSocket::err_display(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpmsgbuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCTSTR)msg, MB_ICONERROR);
	//printf("[%s]%s", (LPCTSTR)msg, lpmsgbuf);
	cout << '[' << msg << ']' << lpmsgbuf;
	LocalFree(lpmsgbuf);
}
SOCKADDR_IN NetworkSocket::GetAddr()
{
	return addr;
}
int NetworkSocket::recvn(char* buf, int len,bool flag)
{
	char* ptr = buf;
	int received = 0;
	int left = len;
	while (left > 0)
	{
		received = recv(sock, ptr, left, flag);
		if (received == SOCKET_ERROR)return SOCKET_ERROR;
		else if (received == 0) return 0;
		left -= received;
		ptr += received;
	}
	return (len - left);
}
