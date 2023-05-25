#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>

atomic<bool> flag;

int main()
{
	flag = false;

	//flag = true;
	flag.store(true, memory_order::memory_order_seq_cst);

	//bool val = flag;
	bool val = flag.load(memory_order::memory_order_seq_cst);

	// 이전 flag 값을 prev에 넣고, flag 값을 수정
	{
		/*bool prev = flag;
		flag = true;*/
		bool prev = flag.exchange(true); // 값을 넣고 true로 바꾸는 과정을 한번에...
	}
	
	// CAS (Compare - And - Swap) 조건부 수정
	{
		bool expected = false;
		bool desired = true;
		
		/*if (flag == expected)
		{
			flag = desired;
			return true;
		}
		else
		{
			expected = flag;
			return false;
		}*/

		flag.compare_exchange_strong(expected, desired);
	}


}
