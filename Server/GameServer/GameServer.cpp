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


class Writhe : public RefCountable
{
public:
	int _hp = 125;
	int _posX = 0;
	int _posY = 0;
};
using WritheRef = TSharedPtr<Writhe>;


class Missile : public RefCountable
{
public:
	void SetTarget(Writhe* target)
	{
		_target = target;
		// 중간에 개입 가능

		//target->AddRef();
		Test(target);
	}

	void Test(WritheRef target)
	{

	}

	bool Update()
	{
		if (_target == nullptr)
			return true;

		int posX = _target->_posX;
		int posY = _target->_posY;

		// TODO : 쫓아간다

		if (_target->_hp == 0)
		{
			//_target->ReleaseRef();
			_target = nullptr;
			return true;
		}

		return false;
	}

	Writhe* _target = nullptr;
};

using MissileRef = TSharedPtr<Missile>;


int main()
{
	WritheRef writhe(new Writhe());
	writhe->ReleaseRef();
	MissileRef missile(new Missile());
	missile->ReleaseRef();

	missile->SetTarget(writhe);

	// 레이스 피격
	writhe->_hp = 0;
	//writhe->ReleaseRef();
	writhe = nullptr;
	//delete writhe;

	while (true)
	{
		if (missile) {
			if (missile->Update())
			{
				//missile->ReleaseRef();
				missile = nullptr;
			}
		}
	}

	missile->ReleaseRef();
	missile = nullptr;
	//delete missile;
}
