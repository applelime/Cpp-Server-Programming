#pragma once
#include <Windows.h>

void CreateSemaphoreAndThreads();
int SemaphoreExample(void);
DWORD WINAPI SemaphoreThreadProc(LPVOID);