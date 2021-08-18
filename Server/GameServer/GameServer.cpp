#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>
#include "ConcurrentStack.h"
#include "ConcurrentQueue.h"
#include "RefCounting.h";

using namespace std;

using KnightRef = TSharedPtr<class Knight>;

class Knight
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	void SetTarget(KnightRef target)
	{
		_target = target;
	}

	KnightRef _target = nullptr;
};



int main()
{
	// 1) 이미 만들어진 클래스 대상으로는 사용 불가
	// 2) 순환 (Cycle) 문제



	// shared_ptr
	// weak_ptr
	// [Knight][RefCountingBlock]
	// [Knight | RefCountingBlock (uses, weak)]
	// [T*][RefCountBlocking*]

	// RefCountBlock(useCount(shared), weakCount) 
	shared_ptr<Knight> spr= make_shared<Knight>();
	weak_ptr<Knight> wpr = spr;

	bool expired = wpr.expired();
	shared_ptr<Knight> spr2 = wpr.lock();

	// [T*][RefCountBlocking*]
	shared_ptr<Knight> spr2 = spr;

	bool expired = wpr.expired();
	shared_ptr<Knight> spr2 = wpr.lock();
	if (spr2 != nullptr)
	{

	}

}
