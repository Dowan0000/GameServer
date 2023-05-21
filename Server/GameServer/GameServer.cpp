﻿#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>

#include "AccountManager.h"
#include "UserManager.h"

void Func1()
{
	for (int32 i = 0; i < 1000; i++)
	{
		UserManager::Instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 i = 0; i < 1000; i++)
	{
		AccountManager::Instance()->ProcessLogin();
	}
}

int main()
{
	std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;

	// 참고 
	mutex m1;
	mutex m2;

	// m1, m2 순서가 아닌 내부적으로 판별해서 일관적인 순서를 보장함
	std::lock(m1, m2);

	//
	lock_guard<mutex> g1(m1, std::adopt_lock);

}
