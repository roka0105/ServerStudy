#include "NetWorkProgram.h"
NetWorkProgram* NetWorkProgram::instance = nullptr;
NetWorkProgram* NetWorkProgram::Instance()
{
	return instance;
}
void NetWorkProgram::Create()
{
	if (instance == NULL)
	{
		instance = new NetWorkProgram();
	}
}
void NetWorkProgram::Destroy()
{
	delete instance;
}
void NetWorkProgram::Init()
{
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
		return;

}
ClientInfo* NetWorkProgram::Connect()
{
	ClientInfo* Client;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVERPORT);
	addr.sin_addr.s_addr = inet_addr(SERVERIP);

	int retval = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR)
		Client->err_quit("connect()");

	Client = new ClientInfo(sock, addr);
	return Client;
}
void NetWorkProgram::End()
{
	WSACleanup();
}
char* NetWorkProgram::R_Packet_Pop()
{
	R_PacketList.Pop_front();
}
void NetWorkProgram::R_Packet_Push(char* data)
{
	R_PacketList.Push_back(data);
}
char* NetWorkProgram::S_Packet_Pop()
{
	S_PacketList.Pop_front();
}
void NetWorkProgram::S_Packet_Push(char* sendbuf)
{
	S_PacketList.Push_back(sendbuf);
}
//DWORD WINAPI NetWorkProgram::SendThread(LPVOID arg)
//{
//	ClientInfo* client = (ClientInfo*)arg;
//	bool endflag = false;
//	while (1)
//	{
//		if (FuntionManager::Instance()->Is_EndProgram())
//		{
//			return 0;
//		}
//		if (!client->sendbuf.is_empty())
//		{
//			client->Send();
//		}
//	}
//	return 0;
//}
DWORD WINAPI NetWorkProgram::NetworkThread(LPVOID arg)
{
	ClientInfo* client = (ClientInfo*)arg;
	bool endflag = false;
	char buf[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	int size = 0;
	while (1)
	{
		if (FuntionManager::Instance()->Is_EndProgram())
		{
			MessageBox(NULL, "네트워크 스레드 정상 종료", "종료", MB_OK);
			return 0;
		}
		if (!client->Recv(buf,size))
		{
			FuntionManager::Instance()->Set_IsEndProgram(true);
			return;
		}
		//*recvbuf 사용하는 곳들에서 list에서 pop해서 가져다 쓰게 해야함.
		NetWorkProgram::Instance()->R_PacketList.Push_back(buf,size);
		size = 0;
		ZeroMemory(buf, MAXBUF);
		if (!NetWorkProgram::Instance()->S_PacketList.is_empty())
		{
			//*sendbuf 사용하는곳들에서 send()하지 않고 list에 push 하도록 변경해야함.
			size = NetWorkProgram::Instance()->S_PacketList[0].datasize;
		    memcpy(buf,NetWorkProgram::Instance()->S_PacketList.Pop_front(),size);
			client->Send(buf, size);
		}

	}
	return 0;
}
NetWorkProgram::NetWorkProgram()
{

}
NetWorkProgram::~NetWorkProgram()
{

}