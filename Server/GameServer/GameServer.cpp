#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

// 참고) cv는 User-Level Object (커널이 아니다!)
condition_variable cv;

void Producer() {
	while (true) 
	{
		// 1) Lock
		// 2) 공유 변수 값 수정
		// 3) Lock 풀고
		// 4) 조건 변수를 통해 다른 스레드에 통지

		{
			unique_lock<mutex> lock(m);
			q.push(100);;
		}

		cv.notify_one(); // wait 중인 스레드가 있으면 딱 1개를 깨운다.
		//this_thread::sleep_for(100000ms);
	}
}

void Consumer() {
	while (true) 
	{
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() { return q.empty() == false; });

		// 1) Lock을 잡고
		// 2) 조건 확인
		// - 만족한다? -> 빠져나와 이어서 코드 실행
		// - 아니다! -> Lock을 풀어주고 대기상태 전환

		// 근데 notify_one을 했으면 항상 조건식을 만족하지 않을까?
		// Spurious Wakeup (가짜 기상?)
		// notify_one 할 때 lock을 잡고 있는게 아니다.

		{
			int32 data = q.front();
			q.pop();
			cout << q.size() << endl;
		}
	}
}

int main()
{
	// 커널 오브젝트
	// Usage Count
	// Signal / Non-Signal << bool
	// Auto // Manual << bool
	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualReset*/, FALSE/*bInitalState*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}

