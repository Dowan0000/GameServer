#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <future>


int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 100'000; i++)
		sum += 1;

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// 동기(synchronous) 실행
	/*int64 sum = Calculate();
	cout << sum << endl;*/

	// std::future
	{
		// 비동기(asynchronous) 실행
		// 1) deferred -> lazy evaluation 지연해서 실행
		// 2) async -> 별도의 쓰레드를 만들어 실행
		// 3) deferred | async -> 둘 중 알아서 실행해주세요

		std::future<int64> future = std::async(std::launch::async, Calculate);
		
		// TODO
		/*future_status status = future.wait_for(1ms);
		if (status == future_status::ready)
		{
		 
		}*/

		int64 sum = future.get(); // 결과물이 필요할 때 받아옴
		
		/*class Knight
		{
		public:
			int64 GetHP() { return 100; }
		};*/

		/*Knight knight;
		std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHP, knight); // knight.GetHP();*/


		// std::promise
		{
			std::promise<string> promise;
			std::future<string> future = promise.get_future();

			thread t(PromiseWorker, std::move(promise));

			string message = future.get();
			cout << message << endl;

			t.join();

		}

		// std::Packaged_task
		{
			std::packaged_task<int64(void)> task(Calculate);
			std::future<int64> future = task.get_future();

			std::thread t(TaskWorker, std::move(task));

			int64 sum = future.get();
			cout << sum << endl;

			t.join();

		}

		// 결론)
		// mutex나 condition_variable까지 가지 않고 단순한 애들을 처리할 수 있는 방법
		// 특히 한 번 발생하는 이벤트에 유용

		// 1) async
		// 원하는 함수를 비동기 실행
		// 2) promise
		// 결과물을 promise를 통해 future로 받아줌
		// 3) packaged_task
		// 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌
	}
	

}
