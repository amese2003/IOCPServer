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


class Knight
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

	/*void* operator new(size_t size)
	{
		cout << "Knight new! " << size << endl;
		void* ptr = ::malloc(size);
		return ptr;
	}

	void operator delete(void* ptr)
	{
		cout << "Knight delete!" << endl;
		::free(ptr);
	}*/

	int32 _hp = 100;
	int32 _mp = 0;

};

// new operator overloading (Global)
//void* operator new(size_t size)
//{
//	cout << "new! " << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete(void* ptr)
//{
//	cout << "delete!" << endl;
//	::free(ptr);
//}
//void* operator new[](size_t size)
//{
//	cout << "new[]! : " << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete[](void* ptr)
//{
//	cout << "delete![]" << endl;
//	::free(ptr);
//}

int main()
{
	Knight* knight = xnew<Knight>(10);
	xdelete(knight);
}
