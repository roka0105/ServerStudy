#include "ClientInfo.h"
ClientInfo::ClientInfo()
{  
	userInfo = new UserInfo();
	is_logout_request = false;
	hStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}
ClientInfo::ClientInfo(SOCKET clientsock, SOCKADDR_IN clientaddr)
{
	userInfo = new UserInfo();
	sock = clientsock;
	memcpy(&addr, &clientaddr, sizeof(SOCKADDR_IN));
	is_logout_request = false;
	hStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}
ClientInfo::ClientInfo(ClientInfo& ref)
{
	this->sock = ref.sock;
	memcpy(&this->addr, &ref.addr, sizeof(SOCKADDR_IN));
	ZeroMemory(buf, MAXBUF);
	memcpy(buf, ref.buf,ref.bufsize);
}
ClientInfo::~ClientInfo()
{
	CloseHandle(hStartEvent);
}
SOCKADDR_IN ClientInfo::GetAddr()
{
	return NetworkSocket::GetAddr();
}
void ClientInfo::Send(PROTOCOL protocol, char* databuf, int size)
{
	NetworkSocket::Send(protocol, databuf, size);
}
bool ClientInfo::Recv(PROTOCOL& protocol, char* databuf)
{
	return NetworkSocket::Recv(protocol, databuf);
}
void ClientInfo::err_display(const char* msg)
{
	NetworkSocket::err_display(msg);
}
void ClientInfo::err_quit(const char* msg)
{
	NetworkSocket::err_quit(msg);
}
void ClientInfo::ClientInfo::SetUserInfo(char* id, char* pw, bool login)
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
void ClientInfo::_SetEvent()
{
	SetEvent(hStartEvent);
}
HANDLE ClientInfo::_GetEvent()
{
	return hStartEvent;
}