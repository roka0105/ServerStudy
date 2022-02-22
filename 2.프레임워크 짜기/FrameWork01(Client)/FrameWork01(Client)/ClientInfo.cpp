#include "ClientInfo.h"
ClientInfo::ClientInfo()
{  
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

	ZeroMemory(buf, MAXBUF);
	int size = ref.bufsize;
	memcpy(buf, ref.buf,size);
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
UserInfo* ClientInfo::GetUserInfo()
{
	return &userInfo;
}
SOCKADDR_IN ClientInfo::GetAddr()
{
	return NetworkSocket::GetAddr();
}
void ClientInfo::Send(const char* databuf, int size)
{
	NetworkSocket::Send(databuf, size);
}
bool ClientInfo::Recv(char* databuf, int& size)
{
	return NetworkSocket::Recv(databuf,size);
}
void ClientInfo::err_display(const char* msg)
{
	NetworkSocket::err_display(msg);
}
void ClientInfo::err_quit(const char* msg)
{
	NetworkSocket::err_quit(msg);
}
void ClientInfo::SetData(const char* data,int size)
{
	const char* ptr = data + sizeof(PROTOCOL);
	memcpy(buf, data, size);
	bufsize = size;
}
void ClientInfo::GetData(char* data, int& size)
{
	size = bufsize;
	memcpy(data, buf, bufsize);
}
void ClientInfo::ClearData()
{
	ZeroMemory(buf, MAXBUF);
	bufsize = 0;
}