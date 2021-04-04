#ifndef __THREADPOOL_H
#define __THREADPOOL_H

#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>
#include <deque>
#include <string>
#include <thread>
#include <algorithm>
namespace hujie
{

	class ThreadPool
	{
	public:
		typedef std::function<void()> Task;

		explicit ThreadPool(const std::string& name = std::string());
		~ThreadPool();

		void start(int numThreads);
		void stop();

		void run(const Task& f);

	private:
		void runInThread();
		Task take();

		std::mutex mutex_;
		std::condition_variable cond_;
		std::string name_;
		std::vector<std::thread> threads_;
		std::deque<Task> queue_;
		bool running_;
	};
}
#endif // !__THREADPOOL_H
