#include "global.h"

bool RecvPacket(SOCKET s, char* recvbuf)
{
	int size = 0;
	
	int retval= recvn(s,(char*)&size, sizeof(int), 0);
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
	int size = 0;
	char* ptr = sendbuf+sizeof(int);
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
int PackPacket(char* sendbuf, const PROTOCOL protocol, const MSGTYPE msgtype, const char* msg)
{
	int size = 0;
	int strsize = 0;
	char* ptr = sendbuf + sizeof(int);
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);
	memcpy(ptr, &msgtype, sizeof(MSGTYPE));
	size += sizeof(MSGTYPE);
	ptr += sizeof(MSGTYPE);
	if (msg == NULL)
	{
		ptr = sendbuf;
		memcpy(ptr, &size, sizeof(int));
		size += sizeof(int);
		return size;
	}
  	strsize = strlen(msg);
	memcpy(ptr, &strsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);
	memcpy(ptr, msg, strsize);
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
int PackPacket(char* sendbuf, const PROTOCOL protocol, const int roomcount, const int limitattendcount, RoomInfo** roomlist, UserInfo* user)
{
	char* ptr = sendbuf+sizeof(int);
	int size = 0;
	int strsize = 0;

	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);
	size += sizeof(PROTOCOL);
	//방 갯수
	memcpy(ptr, &roomcount, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	//인원 제한
	memcpy(ptr, &limitattendcount, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	//방 번호, 문자열갯수,문자열,현재 방에 들어간 인원수
	for (int i = 0; i < roomcount; ++i)
	{
		memcpy(ptr, &roomlist[i]->number, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		strsize = strlen(roomlist[i]->name);
		memcpy(ptr, &strsize, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
		memcpy(ptr, roomlist[i]->name, strsize);
		ptr += strsize;
		size += strsize;
		memcpy(ptr, &roomlist[i]->attend_count, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
	}
	//유저 정보 전송.
	strsize = strlen(user->NICK);
	memcpy(ptr, &strsize, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);
	memcpy(ptr, user->NICK, strsize);
	ptr += strsize;
	size += strsize;

	ptr = sendbuf;
	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);
	return size;
}

void UnPackPacket(const char* recvbuf, int& number)
{
	const char* ptr = recvbuf+sizeof(PROTOCOL);
 	memcpy(&number, ptr, sizeof(int));
}
void UnPackPacket(const char* recvbuf, char* id, char* pw, char* nickname)
{
	const char* ptr = recvbuf + sizeof(PROTOCOL);
	int strsize = 0;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(id, ptr, strsize);
	ptr += strsize;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(pw, ptr, strsize);
	ptr += strsize;
	memcpy(&strsize, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(nickname, ptr, strsize);
}