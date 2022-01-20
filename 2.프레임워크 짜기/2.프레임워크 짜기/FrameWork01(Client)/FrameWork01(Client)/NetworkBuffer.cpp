#include "NetworkBuffer.h"
NetworkBuffer::NetworkBuffer()
{
	ZeroMemory(buf, MAXBUF);
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
}
const char* NetworkBuffer::Data_Pop()
{
	return buf;
}
void NetworkBuffer::Data_Push(char* data)
{
	int strsize = strlen(data);
	memcpy(buf, data, strsize);
}
void NetworkBuffer::Data_Push(char* data, int size)
{
	memcpy(buf, data, size);
}
int NetworkBuffer::Size_Pop()
{
	return size;
}
void NetworkBuffer::Size_Push(int _size)
{
	size = _size;
}
int NetworkBuffer::PackPacket(PROTOCOL protocol)
{
	char* ptr = buf + sizeof(int);
	int size = 0;
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr = buf;
	memcpy(ptr, &size, sizeof(int));
	size += sizeof(int);
	return size;
}
int NetworkBuffer::PackPacket(PROTOCOL protocol,const char* data, int size)
{
	char* ptr = buf+sizeof(int);
	int _size = 0;
	memcpy(ptr, &protocol, sizeof(PROTOCOL));
	_size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);
	memcpy(ptr, data, size);
	_size += size;
	ptr = buf;
	memcpy(ptr, &_size, sizeof(int));
	_size += sizeof(int);
	return _size;
}
void NetworkBuffer::UnPackPacket(PROTOCOL& protocol)
{
	const char* ptr = buf;
	memcpy(&protocol, ptr, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);
}