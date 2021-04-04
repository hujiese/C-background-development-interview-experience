#include "MyThreadPool.h"
#include <cstdio>
#include <cassert>
#include <exception>
using namespace hujie;

ThreadPool::ThreadPool(const std::string& name)
:name_(name),
running_(false)
{
}

ThreadPool::~ThreadPool()
{
	if (running_)
	{
		stop();
	}
}

void ThreadPool::start(int numThreads)
{
	assert(threads_.empty());
	running_ = true;
	threads_.reserve(numThreads);
	for (int i = 0; i < numThreads; ++i)
	{
		char id[32];
		_snprintf_s(id, sizeof id, "%d", i);
		threads_.push_back(std::thread(std::bind(&ThreadPool::runInThread, this), name_ + id));
	}
}

void ThreadPool::stop()
{
	{
		std::unique_lock<std::mutex> lock(mutex_);
		running_ = false;
		cond_.notify_all();
	}
	std::for_each(threads_.begin(), threads_.end(), std::bind(&std::thread::join, std::placeholders::_1));
}

void ThreadPool::run(const Task& task)
{
	if (threads_.empty())
	{
		task();
	}
	else
	{
		std::unique_lock<std::mutex> lock(mutex_);
		queue_.push_back(task);
		cond_.notify_all();
	}
}

ThreadPool::Task ThreadPool::take()
{
	std::unique_lock<std::mutex> lock(mutex_);
	// always use a while-loop, due to spurious wakeup
	while (queue_.empty() && running_)
	{
		cond_.wait(lock);
	}
	Task task;
	if (!queue_.empty())
	{
		task = queue_.front();
		queue_.pop_front();
	}
	return task;
}

void ThreadPool::runInThread()
{
	try
	{
		while (running_)
		{
			Task task(take());
			if (task)
			{
				task();
			}
		}
	}
	catch (const std::exception& ex)
	{
		fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
		fprintf(stderr, "reason: %s\n", ex.what());
		abort();
	}
	catch (...)
	{
		fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
		throw; // rethrow
	}
}

