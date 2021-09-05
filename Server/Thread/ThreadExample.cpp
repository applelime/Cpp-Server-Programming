#include <iostream>
#include <vector>
#include <mutex>
#include "ThreadExample.h"
#include "../Util/PrimeNumber.h"

const int maxCount = 150000;
const int threadCount = 4;

void PrimeNumberSinglethreaded()
{
	vector<int> primes;

	auto startTime = chrono::system_clock::now();
	for (int i = 0; i < maxCount; ++i)
	{
		if (true == IsPrimeNumber(i))
		{
			primes.push_back(i);
		}
	}
	auto endTime = chrono::system_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
	cout << "SingleThread: Took " << duration << " milliseconds." << endl;
}

void PrimeNumberMultithreaded()
{
	int num = 1;
	recursive_mutex numMutex;

	vector<int> primes;
	recursive_mutex primesMutex;

	auto startTime = chrono::system_clock::now();

	// 작동할 워커 스레드
	vector<shared_ptr<thread>> threads;

	for (int i = 0; i < threadCount; ++i)
	{
		shared_ptr<thread> thread(new thread([&]() {
			// 각 스레드의 메인 함수
			while (true)
			{
				int n;
				{
					lock_guard<recursive_mutex> numLock(numMutex);
					n = num;
					num++;
				}

				if (maxCount <= n)
				{
					break;
				}

				if (true == IsPrimeNumber(n))
				{
					lock_guard<recursive_mutex> primesLock(primesMutex);
					primes.push_back(n);
				}
			}
			}));

		// 스레드 객체를 일단 갖고 있는다.
		threads.push_back(thread);
	}

	// 모든 스레드가 일을 마칠 때까지 기다린다.
	for (int i = 0; i < threads.size(); ++i)
	{
		threads[i]->join();
	}

	auto endTime = chrono::system_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
	cout << "MultiThread: Took " << duration << " milliseconds." << endl;
}