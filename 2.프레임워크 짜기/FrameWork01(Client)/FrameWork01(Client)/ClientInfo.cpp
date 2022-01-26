#include "ClientInfo.h"
ClientInfo::ClientInfo()
{  
	Socket();
}
ClientInfo::ClientInfo(SOCKET clientsock, SOCKADDR_IN clientaddr)
{
	sock = clientsock;
	memcpy(&addr, &clientaddr, sizeof(SOCKADDR_IN));
}
ClientInfo::ClientInfo(ClientInfo& ref)
{
	this->sock = ref.sock;
	memcpy(&this->addr, &ref.addr, sizeof(SOCKADDR_IN));
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	int size = ref.sendbuf.Size_Pop();
	memcpy(buf, ref.sendbuf.Data_Pop(),size);
	this->sendbuf.Data_Push(buf,size);
	ZeroMemory(buf, MAXBUF);
	size = ref.recvbuf.Size_Pop();
	memcpy(buf, ref.recvbuf.Data_Pop(),size);
	this->recvbuf.Data_Push(buf,size);
}
ClientInfo::~ClientInfo()
{
	
}
void ClientInfo::SetUserInfo(char* id, char* pw, bool login)
{
	strcpy(userInfo.ID, id);
	strcpy(userInfo.PW, pw);
	userInfo.is_loging = login;
}
