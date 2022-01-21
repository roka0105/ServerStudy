#include "NetworkSocket.h"
NetworkSocket::NetworkSocket()
{
}
NetworkSocket::~NetworkSocket()
{
}
void NetworkSocket::Send(const char*_sendbuf,int size)
{
	int retval = 0;
	retval = send(sock, _sendbuf, size, 0);
	if (retval == SOCKET_ERROR)
		this->err_display("send()");
}
bool  NetworkSocket::Recv()
{
	int retval = 0;
	int size = 0;
	retval= recvn((char*)&size, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
		return false;
	else if (retval == 0)return false;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	retval = recvn(buf, size, 0);
	if (retval == SOCKET_ERROR)
		return false;
	else if (retval == 0)return false;

	this->recvbuf.Data_Push(buf,size);
	return true;
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
