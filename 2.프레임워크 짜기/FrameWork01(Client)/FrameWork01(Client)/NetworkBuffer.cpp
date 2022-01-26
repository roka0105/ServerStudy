#include "NetworkBuffer.h"
NetworkBuffer::NetworkBuffer()
{
	ZeroMemory(buf, MAXBUF);
	size = 0;
}
NetworkBuffer::~NetworkBuffer()
{

}
bool NetworkBuffer::is_empty()
{
	int strsize = strlen(buf);
	if (strsize <= 0)
		return true;
	else return false;
}
void NetworkBuffer::MemoryZero()
{
	ZeroMemory(buf, MAXBUF);
	size = 0;
}
char* NetworkBuffer::Data_Pop()
{
	return buf;
}
//void NetworkBuffer::Data_Push(char* data)
//{
//	int strsize = strlen(data);
//	memcpy(buf, data, strsize);
//}
void NetworkBuffer::Data_Push(char* data, int _size)
{
	memcpy(buf, data, _size);
	size = _size;
}
int& NetworkBuffer::Size_Pop()
{
	return size;
}
void NetworkBuffer::Size_Push(int _size)
{
	size = _size;
}
void NetworkBuffer::PackPacket(PROTOCOL protocol)
{
	char* ptr = buf + sizeof(int);
	int _size = 0;
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	_size += sizeof(PROTOCOL);
	ptr = buf;
	memcpy(ptr, &_size, sizeof(int));
	_size += sizeof(int);
	size = _size;
}
void NetworkBuffer::PackPacket(PROTOCOL protocol,const char* data, int datasize)
{
	char* ptr = buf+sizeof(int);
	int _size = 0;
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	_size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);
	memcpy(ptr, data, datasize);
	_size += datasize;
	ptr = buf;
	memcpy(ptr, &_size, sizeof(int));
	_size += sizeof(int);

	size = _size;
}
void NetworkBuffer::UnPackPacket(PROTOCOL& protocol)
{
	const char* ptr = buf;
	memcpy(&protocol, ptr, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);
}