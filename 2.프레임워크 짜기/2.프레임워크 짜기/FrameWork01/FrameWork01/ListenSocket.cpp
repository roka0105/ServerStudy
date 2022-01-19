#include "ListenSocket.h"
ListenSocket::ListenSocket()
{
}
ListenSocket::~ListenSocket()
{

}
//SOCKET ListenSocket::ListenSock()
//{
//	return sock->GetSock();
//}
void ListenSocket::_Socket(int family, int streamtype)
{
	sock = socket(family, streamtype,0);
}
void ListenSocket::Bind(int family,int ipaddr,int port)
{
	int retval = 0;
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = family;
	addr.sin_addr.s_addr = htonl(ipaddr);
	addr.sin_port = htons(port);

	retval = bind(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (retval == SOCKET_ERROR)
		this->err_quit("bind()");
}
void ListenSocket::Listen()
{
	int retval = 0;
	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		this->err_quit("listen()");
}
SOCKET ListenSocket::Accept(SOCKADDR_IN& output_addr)
{
	Socket* temp = new Socket();
	SOCKET tempsock;
	int addrsize = sizeof(output_addr);
	ZeroMemory(&output_addr, sizeof(SOCKADDR_IN));
	tempsock = accept(sock, (SOCKADDR*)&output_addr, &addrsize);
	return tempsock;
}