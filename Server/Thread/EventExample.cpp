#include <iostream>
#include "EventExample.h"

#define THREADCOUNT 4 

HANDLE writeEvent;
HANDLE eventThreads[THREADCOUNT];

using namespace std;
void CreateEventsAndThreads()
{
	int i;
	DWORD threadID;

	// 수동 리셋 이벤트 생성. 
	writeEvent = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		FALSE,              // initial state is nonsignaled
		TEXT("WriteEvent")  // object name
	);

	if (NULL == writeEvent)
	{
		cout << "CreateEvent failed \n" << GetLastError();
		return;
	}

	// 스레드 생성
	for (i = 0; i < THREADCOUNT; i++)
	{
		eventThreads[i] = CreateThread(
			NULL,              // default security
			0,                 // default stack size
			EventThreadProc,        // name of the thread function
			NULL,              // no thread parameters
			0,                 // default startup flags
			&threadID);

		if (NULL == eventThreads[i])
		{
			cout << "CreateThread failed\n" << GetLastError();
			return;
		}
	}
}

void WriteToBuffer()
{
	cout << "메인스레드에서 이벤트 신호 보냄\n";

	if (false == SetEvent(writeEvent))
	{
		cout << "SetEvent failed\n" << GetLastError();
		return;
	}
}

void CloseEvents()
{
	// 이벤트 파괴
	CloseHandle(writeEvent);
}

int EventExample()
{
	// 이벤트 생성
	CreateEventsAndThreads();

	// 이벤트에 신호룰 준다.
	WriteToBuffer();

	cout << "메인 스레드가 다른 스레드의 종료를 기다리는 중.. \n";
	DWORD waitResult = WaitForMultipleObjects(
		THREADCOUNT,   // number of handles in array
		eventThreads,		// array of thread handles
		TRUE,          // wait until all are signaled
		INFINITE);

	switch (waitResult)
	{
	// 모든 스레드가 신호를 받음
	case WAIT_OBJECT_0:
		cout << "All threads ended, cleaning up for application exit...\n";
		break;

	// 에러 발생
	default:
		cout << "WaitForMultipleObjects failed\n" << GetLastError();
		return 1;
	}

	// 이벤트 파괴
	CloseEvents();
	return 0;
}

DWORD WINAPI EventThreadProc(LPVOID lpParam)
{
	// lpParam not used in this example.
	UNREFERENCED_PARAMETER(lpParam);
	printf("Thread %d waiting for write event...\n", GetCurrentThreadId());
	DWORD waitResult = WaitForSingleObject(
		writeEvent,		// event handle
		INFINITE);		// indefinite wait

	switch (waitResult)
	{
	// 이벤트 개체가 신호를 받음
	case WAIT_OBJECT_0:
		printf("Thread %d reading from buffer\n",
			GetCurrentThreadId());
		break;

	// 에러 발생
	default:
		cout << "Wait Error\n" << GetLastError();
		return 0;
	}

	printf("Thread %d exiting\n", GetCurrentThreadId());
	return 1;
}
