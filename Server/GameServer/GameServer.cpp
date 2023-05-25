#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>


atomic<int64> v;

int main()
{
	// atomic 연산에 한해, 모든 쓰레드가 동일 객체에 대해서 동일한 수정 순서를 관찰 

	cout << v.is_lock_free() << endl; // 1 : CPU or 컴파일러 자체적으로 원자적 연산

	struct Knight
	{
		int32 level;
		int32 hp;
		int32 mp;
	};

	atomic<Knight> K;
	cout << K.is_lock_free() << endl; // 0 : atomic에 의해 원자적 연산

}
