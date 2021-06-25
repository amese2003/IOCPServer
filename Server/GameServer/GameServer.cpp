#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

vector<int32> v;

// Mutual Exclusive (상호 배타적)
mutex m;

//RAII (Resource Acquisition Is Initialization)
template<typename T>
class LockGuard {
public:
	LockGuard(T& m) {
		_mutex = &m;
		_mutex->lock();
	}

	~LockGuard() {
		_mutex->unlock(); 
	}


private:
	T* _mutex;
};

void Push() {
	for (int32 i = 0; i < 10000; i++) 
	{

		//LockGuard<std::mutex> lockGuard(m);
		//std::lock_guard<std::mutex> lockGuard(m);
		std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);

		uniqueLock.lock();
		// 화장실 문 잠구기
		//m.lock();

		v.push_back(i);

		if (i == 5000) {
			break;
		}

		// 자물쇠 푸리
		//m.unlock();
	}
}

int main()
{
	v.reserve(20000);

	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;

}

