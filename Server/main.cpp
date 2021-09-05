#include "main.h"
#include "Thread/ThreadExample.h"
#include "Thread/DeadlockExample.h"
#include "Thread/EventExample.h"
#include "Thread/SemaphoreExample.h"
using namespace std;

int main()
{
	//PrimeNumberSinglethreaded();
	//PrimeNumberMultithreaded();
	//DeadLockExample();
	//EventExample();
	SemaphoreExample();
	return 0;
}