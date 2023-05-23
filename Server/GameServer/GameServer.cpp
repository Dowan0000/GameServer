#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

// 참고) CV는 User-Level Object
condition_variable cv;

// condition_variable_any cv;

void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고, 2) 공유 변수 값을 수정
		// 3) Lock을 풀고, 4) 조건변수 통해 다른 쓰레드에게 통지

		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		cv.notify_one(); // wait중인 쓰레드가 있으면 1개만 깨운다

	}
}

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() {return q.empty() == false; });
		// 1) Lock을 잡고, 2) 조건 확인
		// ~ 만족 -> 빠져나와서 이어서 코드 진행
		// ~ 만족x -> Lock을 풀어주고 대기 상태 -> lock_guard(x), unique_lock(o)

		// notify_one을 했으면 항상 조건식을 만족하는거 아닐까?
		// notify_one -> 얘가 락을 잡기전에 공유변수를 다른애가 건들수 있음
		
		{
			int32 data = q.front();
			q.pop();
			cout << q.size() << endl;
		}
	}
}


int main()
{

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

}
