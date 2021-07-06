﻿#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

int64 result;

int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 100000; i++) {
		sum += i;
	}

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret_Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task) {
	task();
}


int main()
{
	// 동기(synchronus) 실행
	//int64 sum = Calculate();
	//cout << sum << endl;

	// std::future
	{
		// 1) deferred -> lazy evaluation 지연해서 실행하세요
		// 2) async -> 별도의 스레드를 만들어서 실행하세요
		// 3) deferred + async -> 알아서 해라!

		// 언젠가 미래에 결과를 받아줄거다!
		std::future<int64> future = std::async(std::launch::async, Calculate);
		
		// TODO
		

		int64 sum = future.get(); // 결과물이 이제서야 필요하다!

		/*class Knight
		{
		public:
			int64 GetHp() { return 100; }
		};

		Knight knight;
		std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHp, knight);*/
	}

	// std::promise
	{
		// 미래에 결과물을 반환해줄거라 약속
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();

	}

	//std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}

	// 결론 mutex, condition_variable까지 가지 않고 단순한 애들을 처리할 수 있는
	// 특히나, 한번 발생하는 이벤트에 유용하다!
	// 닭잡는데 소잡는 칼을 쓸 필요는 없다!
	// 1) async
	// 원하는 함수를 비동기적으로 실행
	// 2) promise
	// 결과물을 promise를 통해 future로 받아줌
	// 3) packaged_task
	// 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌
	
}

