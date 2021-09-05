#include <iostream>
#include "SemaphoreExample.h"

#define MAX_SEM_COUNT 10
#define THREADCOUNT 12

HANDLE semaphore;
HANDLE semaphoreThreads[THREADCOUNT];

void CreateSemaphoreAndThreads()
{
	DWORD threadID;

	// �������� ����
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

	// ������ ����
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
	// ��������, ������ ����
	CreateSemaphoreAndThreads();

	// �������� ���Ḧ ���
	WaitForMultipleObjects(THREADCOUNT, semaphoreThreads, TRUE, INFINITE);

	// ����
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
		// �������� ���� �õ�
		waitResult = WaitForSingleObject(
			semaphore,   // handle to semaphore
			0L);           // zero-second time-out interval

		switch (waitResult)
		{
		// �������� ������Ʈ ��ȣ ����
		case WAIT_OBJECT_0:
			printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
			isContinue = false;

			// 5ms ���
			Sleep(5);

			// �������� Ǯ���ش�.
			if (false == ReleaseSemaphore(
				semaphore,	  // handle to semaphore
				1,            // increase count by one
				NULL))       // not interested in previous count
			{
				printf("ReleaseSemaphore error: %d\n", GetLastError());
			}
			break;

		// �������� ��ȣ ���ް� Ÿ�Ӿƿ� �߻�
		case WAIT_TIMEOUT:
			printf("Thread %d: wait timed out\n", GetCurrentThreadId());
			break;
		}
	}
	return TRUE;
}