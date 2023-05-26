﻿#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>

//_declspec(thread) int32 value;
thread_local int32 LThreadId = 0;
thread_local queue<int32> q;

void ThreadMain(int32 threadId)
{
	LThreadId = threadId;

	while (true)
	{
		cout << "Hi I'm Thread" << LThreadId << endl;
		this_thread::sleep_for(1s);
	}

}

int main()
{
	/*thread t;
	t.get_id();*/

	vector<thread> threads;

	for (int32 i = 0; i < 10; i++)
	{
		int32 threadId = i + 1;
		threads.push_back(thread(ThreadMain, threadId));
	}

	for (thread& t : threads)
		t.join();

}
