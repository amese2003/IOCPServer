#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

atomic<bool> ready;
int32 value;

void Producer() {
	/*value = 10;
	ready.store(true, memory_order::memory_order_seq_cst);*/

	value = 10;
	ready.store(true, memory_order::memory_order_release);
	// ----------------------- 절취선 -------------------------------------
}

void Consumer() {

	/*while (ready.load(memory_order::memory_order_seq_cst) == false)
		;*/
	
	// ----------------------- 절취선 -------------------------------------
	while (ready.load(memory_order::memory_order_acquire) == false)
		;
	cout << value << endl;
}

int main()
{
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();


	// Memory Model (정책)
	// 1) Sequentially Consistend (seq_cst)
	// 2) Acquire-Release (consume, acquire, release, acq_rel)
	// 3) Relaxed (relaxed)

	// 1) seq_cst (가장 엄격 = 컴파일러 최적화 여지 적음 = 직관적)
	// 가시성, 코드 재배치 문제 해결!
	// 2) acquire-release
	// 딱 중간 단계
	// release 명령 이전의 메모리 명령들이, 헤당 명령 이후로 재배치 되는것 금지
	// 그리고 acquire로 같은 변수를 읽는 쓰레드가 있다면?
	
	// 3) relaxed (자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	// 너무나도 편안하고 자유롭다!
	// 코드 재배치도 멋대로 가능! 가시성 해결도 안됨.
	// 가장 기본 조건 (동일 객체에 대힌 동일 관전 순서만 보장)

	// 인텔, amd경우 애당초 순차적 일관석 보장
	// seq_

	//flag = false;

	//cout << flag.is_lock_free() << endl;


	//// flag = true;
	//flag.store(true, memory_order::memory_order_seq_cst);


	///*bool val = flag;*/
	//bool val = flag.load(memory_order::memory_order_seq_cst);

	//{

	//	bool prev = flag.exchange(true);
	//	//bool prev = flag;
	//	//flag = true;
	//}
	//
	//// CAS (Compare-And-Swap) 조건부 수정
	//{
	//	bool expected = false;
	//	bool desired = true;

	//	flag.compare_exchange_strong(expected, desired);

	//}


	//{

	//	while (true) {
	//		bool expected = false;
	//		bool desired = true;

	//		flag.compare_exchange_weak(expected, desired);
	//	} 
	//}

}

