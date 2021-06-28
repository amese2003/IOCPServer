#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "UserManager.h"
#include "AccountManager.h"

class SpinLock
{
public:
	void lock() {

		// CAS (Compare-And-Swap)

		bool expected = false;
		bool desired = true;

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;

			//this_thread::sleep_for(std::chrono::milliseconds(100));
			//this_thread::sleep_for(100ms);
			this_thread::yield();
		}
	}

	void unlock() {
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false;
};

int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add() {
	for (int i = 0; i < 100000; i++) {
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}
void Sub() {
	for (int i = 0; i < 100000; i++) {
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}
int main()
{

	int32 a = 0;
	a = 1;
	a = 2;
	a = 3;
	a = 4;
	cout << a << endl;

	thread t1(Add);
	thread t2(Sub);
	t1.join();
	t2.join();

	cout << sum << endl;

}

