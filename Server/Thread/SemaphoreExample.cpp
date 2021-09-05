#include <iostream>
#include "SemaphoreExample.h"

#define MAX_SEM_COUNT 10
#define THREADCOUNT 12

HANDLE semaphore;
HANDLE semaphoreThreads[THREADCOUNT];

void CreateSemaphoreAndThreads()
{
	DWORD threadID;

	// 세마포어 생성
	semaphore = CreateSemaphore(
		NULL,           // default security attributes
		MAX_SEM_COUNT,  // initial count
		MAX_SEM_COUNT,  // maximum count
		NULL);          // unnamed semaphore

	if (NULL == semaphore)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return;
	}

	// 스레드 생성
	for (int i = 0; i < THREADCOUNT; i++)
	{
		semaphoreThreads[i] = CreateThread(
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE)SemaphoreThreadProc,
			NULL,       // no thread function arguments
			0,          // default creation flags
			&threadID); // receive thread identifier

		if (NULL == semaphoreThreads[i])
		{
			printf("CreateThread error: %d\n", GetLastError());
			return;
		}
	}
}

int SemaphoreExample()
{
	// 세마포어, 스레드 생성
	CreateSemaphoreAndThreads();

	// 스레드의 종료를 대기
	WaitForMultipleObjects(THREADCOUNT, semaphoreThreads, TRUE, INFINITE);

	// 종료
	for (int i = 0; i < THREADCOUNT; i++)
		CloseHandle(semaphoreThreads[i]);

	CloseHandle(semaphore);

	return 0;
}

DWORD WINAPI SemaphoreThreadProc(LPVOID lpParam)
{

	// lpParam not used in this example
	UNREFERENCED_PARAMETER(lpParam);

	DWORD waitResult;
	bool isContinue = true;

	while (isContinue)
	{
		// 세마포어 진입 시도
		waitResult = WaitForSingleObject(
			semaphore,   // handle to semaphore
			0L);           // zero-second time-out interval

		switch (waitResult)
		{
		// 세마포어 오브젝트 신호 받음
		case WAIT_OBJECT_0:
			printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
			isContinue = false;

			// 5ms 대기
			Sleep(5);

			// 세마포어 풀어준다.
			if (false == ReleaseSemaphore(
				semaphore,	  // handle to semaphore
				1,            // increase count by one
				NULL))       // not interested in previous count
			{
				printf("ReleaseSemaphore error: %d\n", GetLastError());
			}
			break;

		// 세마포어 신호 못받고 타임아웃 발생
		case WAIT_TIMEOUT:
			printf("Thread %d: wait timed out\n", GetCurrentThreadId());
			break;
		}
	}
	return TRUE;
}