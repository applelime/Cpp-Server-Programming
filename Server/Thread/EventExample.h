#pragma once
#include <Windows.h>

void CreateEventsAndThreads(void);
void WriteToBuffer(void);
void CloseEvents(void);
int EventExample(void);
DWORD WINAPI EventThreadProc(LPVOID);