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

class Wraight : public RefCountable
{
public:
	int _hp = 150;
	int _posX = 0;
	int _posY = 0;

};

class Missile : public RefCountable
{
public:
	void SetTarget(Wraight* target)
	{
		_target = target;
		target->AddRef();
	}

	bool Update()
	{
		if (_target == nullptr)
			return true;

		int posX = _target->_posX;
		int posY = _target->_posY;

		// target을 쫓아간다

		// target이 죽으면
		if (_target->_hp == 0)
		{
			_target->ReleaseRef();
			_target = nullptr;
			return true;
		}

		return false;
	}

	Wraight* _target = nullptr;
};

int main()
{
	Wraight* wraight = new Wraight();
	Missile* missile = new Missile();
	missile->SetTarget(wraight);

	// 피격
	wraight->_hp = 0;
	//delete wraight;
	wraight->ReleaseRef();
	wraight = nullptr;

	while (true)
	{
		if (missile)
		{
			if (missile->Update())
			{
				missile->ReleaseRef();
				missile = nullptr;
			}
		}
	}


	//delete missile;
	wraight->ReleaseRef();
	wraight = nullptr;
}
