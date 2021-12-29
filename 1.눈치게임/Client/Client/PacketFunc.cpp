#include "global.h"

bool RecvPacket(SOCKET s, char* recvbuf)
{
	int size = 0;

	int retval = recvn(s, (char*)&size, sizeof(int), 0);
	if (retval == SOCKET_ERROR) return false;
	else if (retval == 0)return false;

	retval = recvn(s, recvbuf, size, 0);
	if (retval == SOCKET_ERROR)return false;
	else if (retval == SOCKET_ERROR)return false;

	return true;
}
PROTOCOL GetProtocol(char* recvbuf)
{
	PROTOCOL protocol;
	memcpy(&protocol, recvbuf, sizeof(PROTOCOL));
	return protocol;
}
int PackPacket(char* sendbuf, const PROTOCOL protocol, const int number)
{
	char* ptr = sendbuf + sizeof(int);
	int size = 0;
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);
	memcpy(ptr, &number, sizeof(int));
	size += sizeof(int);
	ptr = sendbuf;
	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);
	return size;
}
int PackPacket(char* sendbuf, const PROTOCOL protocol, const char* id, const char* pw, const char* nickname)
{
	char* ptr = sendbuf + sizeof(int);
	int size = 0;
	int strsize = 0;

	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);
	strsize = strlen(id);
	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);
	memcpy(ptr, id, strsize);
	size += strsize;
	ptr += strsize;
	strsize = strlen(pw);
	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);
	memcpy(ptr, pw, strsize);
	size += strsize;
	ptr += strsize;
	strsize = strlen(nickname);
	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);
	memcpy(ptr, nickname, strsize);
	size += strsize;
	ptr = sendbuf;
	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);
	return size;
}
int PackPacket(char* sendbuf, const PROTOCOL protocol)
{
	char* ptr = sendbuf + sizeof(int);
	int size = 0;
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr = sendbuf;
	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);
	return size;
}
int PackPacket(char* sendbuf, const PROTOCOL protocol, const char* roomname)
{
	int strsize = 0;
	int size = 0;
	char* ptr = sendbuf + sizeof(int);
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);
	size += sizeof(PROTOCOL);
	strsize = strlen(roomname);
	memcpy(ptr, &strsize, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, roomname, strsize);
	ptr = sendbuf;
	size += strsize;
	memcpy(ptr, &size,sizeof(int));
	size += sizeof(int);
	return size;
}
void UnPackPacket(const char* recvbuf, MSGTYPE* msgtype, char* msg)
{
	int strsize = 0;
	const char* ptr = recvbuf + sizeof(PROTOCOL);
	memcpy(msgtype, ptr, sizeof(MSGTYPE));
	ptr += sizeof(MSGTYPE);
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(msg, ptr, strsize);
}
void UnPackPacket(const char* recvbuf, int& roomcount, int& limitattendcount, RoomInfo* roomlist, UserInfo* userinfo)
{
	const char* ptr = recvbuf + sizeof(PROTOCOL);
	int strsize = 0;
	memcpy(&roomcount, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&limitattendcount, ptr, sizeof(int));
	ptr += sizeof(int);
	for (int i = 0; i < roomcount; ++i)
	{
		memcpy(&roomlist[i].number, ptr, sizeof(int));
		ptr += sizeof(int);
		memcpy(&strsize, ptr, sizeof(int));
		ptr += sizeof(int);
		memcpy(roomlist[i].name, ptr, strsize);
		ptr += strsize;
		memcpy(&roomlist[i].attend_count, ptr, sizeof(int));
		ptr += sizeof(int);
	}
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(userinfo->NICK, ptr, strsize);
}