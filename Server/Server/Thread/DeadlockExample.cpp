#include <thread>
#include <iostream>
#include "DeadlockExample.h"
#include "CriticalSection.h"
using namespace std;

int a;
CriticalSection a_mutex;
int b;
CriticalSection b_mutex;

void DeadLockExample()
{
	thread t1([]()
		{
			while (true)
			{
				CriticalSectionLock lock(a_mutex);
				a++;
				CriticalSectionLock lock2(b_mutex);
				b++;
				cout << "t1 done.\n";
			}
		});

	thread t2([]()
		{
			while (true)
			{
				CriticalSectionLock lock(b_mutex);
				b++;
				CriticalSectionLock lock2(a_mutex);
				a++;
				cout << "t2 done.\n";
			}
		});

	// 스레드들의 일이 끝날 때까지 기다린다.
	t1.join();
	t2.join();
}