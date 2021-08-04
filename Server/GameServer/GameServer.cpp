#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

CoreGlobal Core;

void ThreadMain()
{
	while (true) {
		cout << "나는 스레드...." << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	for (int32 i = 0; i < 5; i++) {
		GThreadManager->Launch(ThreadMain);
	}

	GThreadManager->Join();
}

