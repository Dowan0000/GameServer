#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

int main()
{
	std::thread t;

	auto id1 = t.get_id();
	
	t = std::thread(HelloThread);

	int32 count = t.hardware_concurrency(); // CPU 코어 개수?
	
	auto id2 = t.get_id(); // 쓰레드마다 id

	//t.detach(); // std::thread 객체에서 실제 쓰레드를 분리

	t.joinable(); // 객체가 살아있는지

	if (t.joinable())
	{
		t.join();
	}
	
}
