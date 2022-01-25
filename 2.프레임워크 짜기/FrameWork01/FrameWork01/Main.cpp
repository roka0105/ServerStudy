#include "ProgramManager.h"
int main(int argv,char*argc[])
{
	ProgramManager* Program_Manager=ProgramManager::Instance();
	
	Program_Manager->MainThread();
	return 0;
}
