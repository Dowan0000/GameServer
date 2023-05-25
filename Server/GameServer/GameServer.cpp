#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>

atomic<bool> ready;
int32 value;

void Producer()
{
	value = 10;

	ready.store(true, memory_order::memory_order_release);
	// -------------- 절취선 ----------------
	// atomic_thread_fence(memory_order::memory_order_release);
}

void Consumer()
{
	// atomic_thread_fence(memory_order::memory_order_acquire);
	// -------------- 절취선 ------------------
	while (ready.load(memory_order::memory_order_acquire) == false)
		;

	cout << value << endl;
}

int main()
{
	// Memory Model (정책)
	// 1) Sequentially Consistent (seq_cst)
	// 2) Acquire-Release (acquire, release)
	// 3) Relaxed

	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();


	// 1) seq_cst (가장 엄격 : 컴파일러 최적화 여지가 적음 = 직관적)
	//		- 가시성문제, 코드재배치문제 바로 해결됨


	// 2) acquire-release
	//		- 중간
	//		- release 명령 이전의 메모리 명령들이, 해당 명령 이후로 재배치 되는 것을 금지
	//		- 그리고 acquire로 같은 변수를 읽는 쓰레드가 있다면,
	//		   release 이전의 명령들이 acquire 이후에 관찰 가능 (가시성 보장)


	// 3) relaxed (자유롭다 : 컴파일러 최적화 여지가 많음 = 직관적이지 않음)
	//		- 코드재배치 제멋대로 가능, 가시성 해결안됨
	//		- 가장 기본 조건 (동일 객체에 대한 동일 관전 순서만 보장)
	//		- 거의 사용하지 않음



}
