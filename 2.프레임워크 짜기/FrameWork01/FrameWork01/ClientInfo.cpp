#include "ClientInfo.h"
ClientInfo::ClientInfo()
{  
	userInfo = new UserInfo();
	is_logout_request = false;
}
ClientInfo::ClientInfo(SOCKET clientsock, SOCKADDR_IN clientaddr)
{
	userInfo = new UserInfo();
	sock = clientsock;
	memcpy(&addr, &clientaddr, sizeof(SOCKADDR_IN));
	is_logout_request = false;
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
void ClientInfo::SetUserInfo(char* id, char* pw, bool login)
{
	strcpy(userInfo->ID, id);
	strcpy(userInfo->PW, pw);
	userInfo->is_loging = login;
}
UserInfo* ClientInfo::GetUserInfo()
{
	return userInfo;
 }
void ClientInfo::LogOut()
{
	is_logout_request = true;
}
bool  ClientInfo::LogOutRequest()
{
	bool check = is_logout_request;
	is_logout_request = false;
	return check;
}