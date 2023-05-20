#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>

vector<int32> v;

// Mutual Exclusive
mutex m;

void Push()
{
	for (int32 i = 0; i < 10'000; i++)
	{
		m.lock();
		v.push_back(i);
		m.unlock();
	}
		
}

int main()
{
	//v.reserve(20000);

	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;

}
