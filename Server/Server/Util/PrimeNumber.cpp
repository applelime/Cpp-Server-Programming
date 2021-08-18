#include <iostream>
#include <vector>
#include "PrimeNumber.h"
using namespace std;

bool IsPrimeNumber(int number)
{
	if (1 == number)
	{
		return false;
	}
	else if (2 == number || 3 == number)
	{
		return true;
	}

	for (int i = 2; i < number - 1; ++i)
	{
		if (0 == number % i)
		{
			return false;
		}
	}
	return true;
}

void PrintNumbers(const vector<int>& primes)
{
	for (int i = 0; i < primes.size(); ++i)
	{
		cout << primes[i] << endl;
	}
}