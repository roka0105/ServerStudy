#include "MultiThread.h"
void MultiThread::Loop()
{
	this->Init();
	while (1)
	{
		void* client=AcceptClient();
		MainMember* Main_mem = new MainMember();
		Main_mem->This = this;
		Main_mem->client = client;
		
		HANDLE handle = CreateThread(NULL, 0, ThreadFunc, Main_mem, 0, NULL);
		Err(handle, Main_mem);
	}
	this->End();
}
DWORD __stdcall MultiThread::ThreadFunc(LPVOID ptr)
{
	MainMember* mainptr = (MainMember*)ptr;
	mainptr->This->Run(ptr);
	return 0;
}