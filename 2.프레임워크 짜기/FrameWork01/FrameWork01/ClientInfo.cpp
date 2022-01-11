#include "ClientInfo.h"
ClientInfo::ClientInfo()
{  

}
ClientInfo::ClientInfo(SOCKET clientsock, SOCKADDR_IN clientaddr)
{
	sock = clientsock;
	memcpy(&addr, &clientaddr, sizeof(SOCKADDR_IN));
}
ClientInfo::ClientInfo(const ClientInfo& ref)
{
	this->sock = ref.sock;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	memcpy(&this->addr, &ref.addr, sizeof(SOCKADDR_IN));
	memcpy(buf, ref.sendbuf.Data_Pop(),sizeof(ref.sendbuf.Size_Pop()));
	this->sendbuf.Data_Push(buf);
}
ClientInfo::~ClientInfo()
{
	
}
