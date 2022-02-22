#include "NetworkSocket.h"
NetworkSocket::NetworkSocket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
}
NetworkSocket::~NetworkSocket()
{
	closesocket(sock);
}
void NetworkSocket::Send(PROTOCOL protocol, char* databuf, int size)
{
	int retval = 0;
	int _size = 0;
	
	char* ptr = buf + sizeof(int);
	ZeroMemory(buf, MAXBUF);
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	_size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);

	if (databuf != nullptr)
	{
		memcpy(ptr, databuf, size);
		_size += size;
		ptr += size;
	}
	ptr = buf;
	memcpy(ptr, &_size, sizeof(int));
	_size += sizeof(int);
	retval = send(sock, buf, _size, 0);
	if (retval == SOCKET_ERROR)
		this->err_display("send()");
	ZeroMemory(buf, MAXBUF);
}
bool  NetworkSocket::Recv(PROTOCOL& protocol, char* data)
{
	int retval = 0;
	int size = 0;
	retval = recvn((char*)&size, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
		return false;
	else if (retval == 0)return false;

	ZeroMemory(buf, MAXBUF);
	retval = recvn(buf, size, 0);
	if (retval == SOCKET_ERROR)
		return false;
	else if (retval == 0)return false;

	memcpy(&protocol, buf, sizeof(PROTOCOL));
	memcpy(data, buf + sizeof(PROTOCOL), size - sizeof(PROTOCOL));
	ZeroMemory(buf, MAXBUF);
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
int NetworkSocket::recvn(char* buf, int len, bool flag)
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
