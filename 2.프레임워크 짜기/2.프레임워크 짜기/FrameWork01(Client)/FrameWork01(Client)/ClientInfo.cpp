#include "ClientInfo.h"
ClientInfo::ClientInfo()
{  
	Socket();
}
ClientInfo::ClientInfo(SOCKET clientsock, SOCKADDR_IN clientaddr)
{
	Socket(clientsock, clientaddr);
}
ClientInfo::ClientInfo(ClientInfo& ref)
{
	this->sock = ref.sock;
	memcpy(&this->addr, &ref.addr, sizeof(SOCKADDR_IN));
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	memcpy(buf, ref.sendbuf.Data_Pop(),ref.sendbuf.Size_Pop());
	this->sendbuf.Data_Push(buf);
	ZeroMemory(buf, MAXBUF);
	memcpy(buf, ref.recvbuf.Data_Pop(),ref.recvbuf.Size_Pop());
	this->recvbuf.Data_Push(buf);
}
ClientInfo::~ClientInfo()
{
	
}
