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

	// �۵��� ��Ŀ ������
	vector<shared_ptr<thread>> threads;

	for (int i = 0; i < threadCount; ++i)
	{
		shared_ptr<thread> thread(new thread([&]() {
			// �� �������� ���� �Լ�
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

		// ������ ��ü�� �ϴ� ���� �ִ´�.
		threads.push_back(thread);
	}

	// ��� �����尡 ���� ��ĥ ������ ��ٸ���.
	for (int i = 0; i < threads.size(); ++i)
	{
		threads[i]->join();
	}

	auto endTime = chrono::system_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
	cout << "MultiThread: Took " << duration << " milliseconds." << endl;
}