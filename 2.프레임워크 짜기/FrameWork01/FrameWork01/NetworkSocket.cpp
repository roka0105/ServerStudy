#include "NetworkSocket.h"
NetworkSocket::NetworkSocket()
{
	sock = new Socket();
}
NetworkSocket::NetworkSocket(const NetworkSocket& ref)
{
	memcpy(sock, ref.sock,sizeof(Socket));
}
NetworkSocket::~NetworkSocket()
{
	sock->~Socket();
}
Socket* NetworkSocket::Sock()
{
	return sock;
}
void NetworkSocket::Send(NetworkBuffer _sendbuf)
{
	int retval = 0;
	retval = send(sock->GetSock(), _sendbuf.Data_Pop(), _sendbuf.Size_Pop(), 0);
	if (retval == SOCKET_ERROR)
		sock->err_display("send()");
}
bool  NetworkSocket::Recv(NetworkBuffer _recvbuf)
{
	int retval = 0;
	int size = 0;
	retval= recvn((char*)&size, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
		return false;
	else if (retval == 0)return false;
	retval = recvn(_recvbuf.Data_Pop(), size, 0);
	if (retval == SOCKET_ERROR)
		return false;
	else if (retval == 0)return false;

	return true;
}
int NetworkSocket::recvn(char* buf, int len,bool flag)
{
	char* ptr = buf;
	int received = 0;
	int left = len;
	while (left > 0)
	{
		received = recv(sock->GetSock(), ptr, left, flag);
		if (received == SOCKET_ERROR)return SOCKET_ERROR;
		else if (received == 0) return 0;
		left -= received;
		ptr += received;
	}
	return (len - left);
}
