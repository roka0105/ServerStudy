#define MAIN
#include "global.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	hInst= hInstance;
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (hReadEvent == NULL)return 1;
	hWriteEvent=CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL)return 1;
	hWaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWaitEvent == NULL)return 1;
	hThread[0] = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc1);

	WaitForMultipleObjects(2, hThread, true, INFINITE);
	CloseHandle(hThread[0]);
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);
	CloseHandle(hWaitEvent);

	closesocket(Client->sock);
	delete Client->user;
	delete Client;
	WSACleanup();
	return 0;
}