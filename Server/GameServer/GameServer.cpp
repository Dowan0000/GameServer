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


using KnightRef = TSharedPtr<class Knight>;
using InventoryRef = TSharedPtr<class Inventory>;

class Knight : public RefCountable
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

	void SetTarget(KnightRef target)
	{
		_target = target;
	}

	KnightRef _target = nullptr;
	InventoryRef _inventory = nullptr;
};

class Inventory : public RefCountable
{
public:
	Inventory(KnightRef knight) : _knight(**knight)
	{

	}

	Knight& _knight;
};

int main()
{
	// 순환(Cycle) 문제

	/*KnightRef k1(new Knight());
	k1->ReleaseRef();
	KnightRef k2(new Knight());
	k2->ReleaseRef();

	k1->SetTarget(k2);
	k2->SetTarget(k1);

	k1 = nullptr;
	k2 = nullptr;*/

	KnightRef k1(new Knight());
	k1->ReleaseRef();

	k1->_inventory = new Inventory(k1);

}
