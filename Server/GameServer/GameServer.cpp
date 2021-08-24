#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>
#include "Memory.h"

#include "Allocator.h"
#include "ThreadManager.h"
#include "LockFreeStack.h"

using namespace std;

class Player
{
public:
	Player() {};
	virtual ~Player() {};
};


class Knight : public Player
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	int32 _hp = 100;
	int32 _mp = 0;

};

DECLSPEC_ALIGN(16)
class Data
{
public:
	SListEntry _entry;
	int64 _rand = rand() % 1000;
};

SListHeader* GHeader;

int main()
{
	GHeader = new SListHeader();
	ASSERT_CRASH(((uint64)GHeader % 16) == 0);
	InitializeHead(GHeader);

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]() {
			while (true)
			{
				Data* data = new Data();
				ASSERT_CRASH(((uint64)data % 16) == 0);
				PushEntrySList(GHeader, (SListEntry*)data);
				this_thread::sleep_for(10ms);
			}
		});
	}

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]() {
			while (true)
			{
				Data* pop = nullptr;
				pop = (Data*)PopEntrySList(GHeader);

				if (pop)
				{
					cout << pop->_rand << endl;
					delete pop;
				}
				else
				{
					cout << "NONE" << endl;
				}
			}
		});
	}

	GThreadManager->Join();
}
