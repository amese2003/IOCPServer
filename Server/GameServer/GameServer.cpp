#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "PlayerManager.h"
#include "AccountManager.h"

using namespace std;

vector<bool> numbers;
mutex lock;

// 소수 구하기 (멀티 스레딩)
bool IsPrime(int num) 
{
	if (num <= 1)
		return false;

	if (num == 2)
		return true;

	for (int i = 2; i < num; i++) 
	{
		if (num % i == 0)
			return false;
	}

	return true;
}
//
//int GetPrimeCount(int start, int end)
//{
//	int count = 0;
//
//
//	
//
//	for (int i = start; i < end; i++) {
//		if (numbers[i] == false)
//			continue;
//
//		int sum = i - start + 2;
//
//		for (int i = start; start <= end; i += sum)
//		{
//			if (i % sum == 0)
//				numbers[i] = false;
//		}
//
//		if (numbers[i] == true)
//			count++;
//	}
//
//	return count;
//}

int GetPrimeCount(int start, int end)
{
	int count = 0;

	for (int number = start; number <= end; number++)
	{
		if (IsPrime(number))
			count++;
	}

	return count;
}



int main()
{
	const int MAX_NUMBER = 100'0000;
	atomic<int> primeCount = 0;

	// 1 ~ MAX_NUMBER까지의 소수 개수
	numbers = vector<bool>(MAX_NUMBER + 1, true);

	//int ThreadCount = 3;
	int ThreadCount = thread::hardware_concurrency();
	int unit = (MAX_NUMBER / ThreadCount) + 1;

	vector<thread> threads;

	for (int i = 0; i < ThreadCount; i++) {
		int start = (i * unit) + 1;
		int end = min(MAX_NUMBER, (i + 1) * unit);

		threads.push_back(thread([start, end, &primeCount]() {  primeCount += GetPrimeCount(start, end);  }));
	}

	for (int i = 0; i < threads.size(); i++) 
		threads[i].join();
	
	cout << primeCount;
}

