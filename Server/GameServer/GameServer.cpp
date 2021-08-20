#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>
#include "Memory.h"

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

int main()
{
	//Knight* k1 = new Knight;
	//k1->_hp = 200;
	//k1->_mp = 50;
	//delete k1;
	//k1 = nullptr;
	//k1->_hp = 100; // ? Use-After-Free

	//vector<int32> v{ 1,2,3,4,5 };
	//for (int32 i = 0; i < 5; i++) {
	//	int32 value = v[i];
	//	// TODO
	//	if (value == 3)
	//	{
	//		v.clear();
	//	}
	//}

	//Player* p = new Player();
	//Knight* k = static_cast<Knight*>(p);

	//k->_hp = 200; // ?

	// 가상 메모리 기본
	//int* num = new int;
	//*num = 100;

	//// 유저레벨 (메모장, 롤, 서버)
	//// ----------------------
	//// 커널레벨 (OS Code)

	//// 2GB [                                    ]
	//// 2GB / 4KB [r][w][rw][x][][][][][][]

	//SYSTEM_INFO info;
	//::GetSystemInfo(&info);

	//info.dwPageSize; // 4kb (0x1000)
	//info.dwAllocationGranularity; // 64kb (0x10000)

	////
	//Knight* test = new Knight();
	////int* test = (int*)::VirtualAlloc(NULL, 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//test->_hp = 100;
	//delete test;
	////::VirtualFree(test, 0, MEM_RELEASE);

	//test->_hp = 200;

	Knight* knight = (Knight*)xnew<Player>();
	xdelete(knight);
	knight->_hp = 100;
}
