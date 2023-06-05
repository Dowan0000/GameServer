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
#include "Memory.h"
#include "Allocator.h"

class Knight
{
public:
	Knight()
	{
		cout << " 생성 " << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight HP : " << _hp << endl;
	}

	~Knight()
	{
		cout << " 소멸 " << endl;
	}

	int32 _hp = 100;
	int32 _mp = 10;

};


// new operator overloading (Global)
void* operator new(size_t size)
{
	cout << "new operator overloading" << size << endl;
	void* ptr = malloc(size);
	return ptr;
}

void operator delete(void* ptr)
{
	cout << "delete operator overloading" << endl;
	free(ptr);
}

void* operator new[](size_t size)
{
	cout << "new[] operator overloading" << size << endl;
	void* ptr = malloc(size);
	return ptr;
}

void operator delete[](void* ptr)
{
	cout << "delete[] operator overloading" << endl;
	free(ptr);
}


int main()
{
	//vector<Knight, STLAllocator<Knight>> v(100);
	Vector<Knight> v(100);

	map<int32, int32> m;

}
