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
void NetworkSocket::Send(NetworkBuffer sendbuf)
{
	int retval = 0;
	retval = send(sock->GetSock(), sendbuf.Data_Pop(), sendbuf.Size_Pop(), 0);
	if (retval == SOCKET_ERROR)
		sock->err_display("send()");
}
void NetworkSocket::Recv(NetworkBuffer recvbuf)
{

}