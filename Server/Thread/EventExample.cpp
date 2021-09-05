#include <iostream>
#include "EventExample.h"

#define THREADCOUNT 4 

HANDLE writeEvent;
HANDLE threads[THREADCOUNT];

using namespace std;
void CreateEventsAndThreads()
{
	int i;
	DWORD threadID;

	// ���� ���� �̺�Ʈ ����. 
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

	// ������ ����
	for (i = 0; i < THREADCOUNT; i++)
	{
		threads[i] = CreateThread(
			NULL,              // default security
			0,                 // default stack size
			ThreadProc,        // name of the thread function
			NULL,              // no thread parameters
			0,                 // default startup flags
			&threadID);

		if (NULL == threads[i])
		{
			cout << "CreateThread failed\n" << GetLastError();
			return;
		}
	}
}

void WriteToBuffer()
{
	cout << "���ν����忡�� �̺�Ʈ ��ȣ ����\n";

	if (false == SetEvent(writeEvent))
	{
		cout << "SetEvent failed\n" << GetLastError();
		return;
	}
}

void CloseEvents()
{
	// �̺�Ʈ �ı�
	CloseHandle(writeEvent);
}

int EventExample()
{
	// �̺�Ʈ ����
	CreateEventsAndThreads();

	// �̺�Ʈ�� ��ȣ�� �ش�.
	WriteToBuffer();

	cout << "���� �����尡 �ٸ� �������� ���Ḧ ��ٸ��� ��.. \n";
	DWORD waitResult = WaitForMultipleObjects(
		THREADCOUNT,   // number of handles in array
		threads,		// array of thread handles
		TRUE,          // wait until all are signaled
		INFINITE);

	switch (waitResult)
	{
	// ��� �����尡 ��ȣ�� ����
	case WAIT_OBJECT_0:
		cout << "All threads ended, cleaning up for application exit...\n";
		break;

	// ���� �߻�
	default:
		cout << "WaitForMultipleObjects failed\n" << GetLastError();
		return 1;
	}

	// �̺�Ʈ �ı�
	CloseEvents();
	return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	// lpParam not used in this example.
	UNREFERENCED_PARAMETER(lpParam);
	printf("Thread %d waiting for write event...\n", GetCurrentThreadId());
	DWORD waitResult = WaitForSingleObject(
		writeEvent,		// event handle
		INFINITE);		// indefinite wait

	switch (waitResult)
	{
	// �̺�Ʈ ��ü�� ��ȣ�� ����
	case WAIT_OBJECT_0:
		printf("Thread %d reading from buffer\n",
			GetCurrentThreadId());
		break;

	// ���� �߻�
	default:
		cout << "Wait Error\n" << GetLastError();
		return 0;
	}

	printf("Thread %d exiting\n", GetCurrentThreadId());
	return 1;
}
