#include "ClientInfo.h"
ClientInfo::ClientInfo()
{  

}
ClientInfo::ClientInfo(NetworkSocket* clientsock)
{
	sock = clientsock;
}
ClientInfo::ClientInfo(const ClientInfo& ref)
{

}
ClientInfo::~ClientInfo()
{
	delete sock;
}
SOCKET ClientInfo::Get_Sock()
{
	return sock->Sock()->GetSock();
}
void ClientInfo::Send()
{
	sock->Send(sendbuf);
}
void ClientInfo::Recv()
{

}