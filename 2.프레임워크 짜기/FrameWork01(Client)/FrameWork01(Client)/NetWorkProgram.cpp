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
void NetWorkProgram::R_Packet_Pop(char* recvbuf, int& size)
{
	char* buf = R_PacketList.Pop_front(size);
	memcpy(recvbuf, buf, size);
	delete buf;
}
void NetWorkProgram::R_Packet_Push(const char* data, int size)
{
	char* buf = new char[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	memcpy(buf, data, size);
	R_PacketList.Push_back(buf, size);
}
bool NetWorkProgram::PacketList_IsEmpty(bool recv)
{
	if (recv)
	{
		return R_PacketList.is_empty();
	}
	else return S_PacketList.is_empty();
}
void NetWorkProgram::S_Packet_Pop(char* buf, int& size)
{
	char* _buf = S_PacketList.Pop_front(size);
	memcpy(buf, _buf, size);
	delete _buf;
}
void NetWorkProgram::S_Packet_Push(const char* sendbuf, int size)
{
	char* buf=new char[MAXBUF];
	ZeroMemory(buf, MAXBUF);
	memcpy(buf, sendbuf,size);
	S_PacketList.Push_back(buf, size);
}
DWORD WINAPI NetWorkProgram::RecvThread(LPVOID arg)
{
	ClientInfo* client = (ClientInfo*)arg;
	bool endflag = false;
	
	int size = 0;
	while (1)
	{
		char buf[MAXBUF];
		ZeroMemory(buf, MAXBUF);
		/*if (FuntionManager::Instance()->Is_EndProgram())
		{
			MessageBox(NULL, "recv 스레드 정상 종료", "종료", MB_OK);
			return 0;
		}*/
		if (!client->Recv(buf, size))
		{
			FuntionManager::Instance()->Set_IsEndProgram(true);
			MessageBox(NULL, "recv 스레드 정상 종료", "종료", MB_OK);
			return 0;
		}
		//*recvbuf 사용하는 곳들에서 list에서 pop해서 가져다 쓰게 해야함.
		NetWorkProgram::Instance()->R_Packet_Push(buf,size);
	}
	return 0;
}
DWORD WINAPI NetWorkProgram::SendThread(LPVOID arg)
{
	ClientInfo* client = (ClientInfo*)arg;
	bool endflag = false;

	int size = 0;
	while (1)
	{
		char buf[MAXBUF];
		ZeroMemory(buf, MAXBUF);
		if (FuntionManager::Instance()->Is_EndProgram())
		{
			MessageBox(NULL, "send 스레드 정상 종료", "종료", MB_OK);
			return 0;
		}
		if (!NetWorkProgram::Instance()->S_PacketList.is_empty())
		{
			//*sendbuf 사용하는곳들에서 send()하지 않고 list에 push 하도록 변경해야함.
			//size = NetWorkProgram::Instance()->S_PacketList[0].datasize;
			NetWorkProgram::Instance()->S_Packet_Pop(buf, size);
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