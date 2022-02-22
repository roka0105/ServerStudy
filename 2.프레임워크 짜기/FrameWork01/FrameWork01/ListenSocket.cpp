#include "ListenSocket.h"
ListenSocket::ListenSocket()
{
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
}
ListenSocket::~ListenSocket()
{
	closesocket(sock);
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
	SOCKET tempsock;
	int addrsize = sizeof(output_addr);
	ZeroMemory(&output_addr, sizeof(SOCKADDR_IN));
	tempsock = accept(sock, (SOCKADDR*)&output_addr, &addrsize);
	return tempsock;
}
SOCKADDR_IN ListenSocket::GetAddr()
{
	return addr;
}
void ListenSocket::err_quit(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpmsgbuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(1);
}
void ListenSocket::err_display(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpmsgbuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCTSTR)msg, MB_ICONERROR);
	//printf("[%s]%s", (LPCTSTR)msg, lpmsgbuf);
	cout << '[' << msg << ']' << lpmsgbuf;
	LocalFree(lpmsgbuf);
}