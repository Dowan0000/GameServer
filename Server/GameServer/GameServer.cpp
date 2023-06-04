#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>
#include <vector>

#include "ThreadManager.h"
#include "RefCounting.h"



class Knight
{
public:
	Knight()
	{
		cout << " 생성 " << endl;
	}
	~Knight()
	{
		cout << " 소멸 " << endl;
	}

	
};



int main()
{
	// 순환(Cycle) 문제

	// unique_ptr
	// shared_ptr
	// weak_ptr

	//shared_ptr<Knight> spr(new Knight());
	shared_ptr<Knight> spr = make_shared<Knight>();
	weak_ptr<Knight> wpr = spr;

	bool expired = wpr.expired();
	shared_ptr<Knight> spr2 = wpr.lock();
	if (spr2 != nullptr)
	{

	}
}
