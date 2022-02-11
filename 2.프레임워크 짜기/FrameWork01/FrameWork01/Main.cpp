#include "MainManager.h"
int main(int argv,char*argc[])
{
	MainManager::Create();
	MainManager* Program_Manager=MainManager::Instance();
	
	Program_Manager->Loop();
	MainManager::Destroy();
	return 0;
}