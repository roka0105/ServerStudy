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
int NetworkBuffer::Size_Pop()
{
	int strsize = strlen(buf);
	return strsize;
}
int NetworkBuffer::PackPacket(PROTOCOL protocol, char* data, int size)
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
void NetworkBuffer::UnPackPacket(PROTOCOL& protocol, char* data)
{

}