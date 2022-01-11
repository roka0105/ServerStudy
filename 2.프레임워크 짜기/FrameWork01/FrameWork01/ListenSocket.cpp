#include "ListenSocket.h"
ListenSocket::ListenSocket()
{
	sock = new Socket();
}
ListenSocket::~ListenSocket()
{
	sock->~Socket();
}
//SOCKET ListenSocket::ListenSock()
//{
//	return sock->GetSock();
//}
void ListenSocket::_Socket(int family, int streamtype)
{
	SOCKET _sock = sock->GetSock();
	_sock = socket(family, streamtype);
}
void ListenSocket::Bind(int family,int ipaddr,int port)
{
	SOCKET _sock=sock->GetSock();
	SOCKADDR_IN* addr = NULL;
	int retval = 0;
	memcpy(addr, &(sock->GetAddr()), sizeof(SOCKADDR_IN));
	addr->sin_family = family;
	addr->sin_addr.s_addr = htonl(ipaddr);
	addr->sin_port = htons(port);

	retval = bind(_sock, (SOCKADDR*)addr, sizeof(SOCKADDR_IN));
	if (retval == SOCKET_ERROR)
		sock->err_quit("bind()");
}
void ListenSocket::Listen()
{
	int retval = 0;
	SOCKET _sock = sock->GetSock();
	retval = listen(_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		sock->err_quit("listen()");
}
NetworkSocket* ListenSocket::Accept()
{
	NetworkSocket* client = new NetworkSocket();
	Socket* _sock = client->Sock();
	SOCKET tempsock;
	SOCKADDR_IN tempaddr;
	int addrsize = 0;
	ZeroMemory(&tempaddr, sizeof(SOCKADDR_IN));
	tempsock = accept(sock->GetSock(), (SOCKADDR*)&tempaddr, &addrsize);
	_sock->SetSock(tempsock);
	_sock->SetAddr(tempaddr);
	return client;
}
Socket* NetworkSocket::Sock()
{
	return sock;
}