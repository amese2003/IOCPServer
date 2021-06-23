#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>

void HelloThread() 
{
	cout << "Hello Thread" << endl;
}

void HelloThread2(int32 num)
{
	cout << num << endl;
}

int main()
{
	vector<std::thread> v;
	//v.resize(10);

	//std::thread t;
	//auto id1 = t.get_id();	// 스레드 마다 id;;


	//t = std::thread(HelloThread);

	//int32 count = t.hardware_concurrency(); // cpu 코어 개수?
	//auto id2 = t.get_id();	// 스레드 마다 id

	//std::thread t2;

	//t2 = std::thread(HelloThread2, 1);
	//
	////t.detach(); // std::thread 객체에서 실제 스레드를 분리\


	//if (t.joinable())
	//	t.join();

	//t2.join();

	for (int32 i = 0; i < 10; i++) {
		v.push_back(std::thread(HelloThread2, i));
	}

	for (int32 i = 0; i < 10; i++) {
		if (v[i].joinable())
			v[i].join();
	}

	cout << "Hello Main" << endl;
}

