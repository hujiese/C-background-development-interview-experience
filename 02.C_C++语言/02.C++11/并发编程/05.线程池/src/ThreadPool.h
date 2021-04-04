#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

// �̳߳���
class ThreadPool {
public:
	// ���캯���������߳���
	ThreadPool(size_t threads);
	// �������(���뺯���ͺ����Ĳ���)
	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
		->std::future<typename std::result_of<F(Args...)>::type>;
	// һ����򵥵ĺ�����װģ���������д(C++11)�������κκ���(��Ρ���Ա������)
	// template<class F, class... Args>
	// auto enqueue(F&& f, Args&&... args) -> decltype(declval<F>()(declval<Args>()...))
	// {    return f(args...); }
	// C++14����
	// template<class F, class... Args>
	// auto enqueue(F&& f, Args&&... args)
	// {    return f(args...); }

	// ����
	~ThreadPool();
private:
	// need to keep track of threads so we can join them
	// �����߳���
	std::vector< std::thread > workers;
	// �������
	std::queue< std::function<void()> > tasks;

	// synchronization �첽
	std::mutex queue_mutex; // ���л�����
	std::condition_variable condition;  // ��������
	bool stop;  // ֹͣ��־
};

// the constructor just launches some amount of workers
// ���캯��������һЩ�����߳�
inline ThreadPool::ThreadPool(size_t threads)
: stop(false)
{
	for (size_t i = 0; i<threads; ++i)
		// ����̵߳������߳���
		workers.emplace_back(   // ��push_back���ͣ������ܸ���(������ƵĻ���emplace/emlace_front)
		[this]
	{   // �߳��ڲ��ϵĴ��������ȡ����ִ��
		for (;;)
		{
			std::function<void()> task;

			{
				// ����(��ռ����Ȩʽ)
				std::unique_lock<std::mutex> lock(this->queue_mutex);
				// �ȴ���������
				this->condition.wait(lock,
					[this]{ return this->stop || !this->tasks.empty(); });
				// ִ�����������ȴ���ʱ���Ѿ��õ�����(��lock�Ѿ��õ�����û������)
				// ���ｫ��unlock�ͷ����������߳̿��Լ������������˴���Ȼ�������ȴ���������
				// һ���յ������߳�notify_*���ѣ����ٴ�lock��Ȼ����������ж�
				// ��[return this->stop || !this->tasks.empty()]�Ľ��Ϊfalse������
				// ����Ϊtrueʱ������������ʱlock��ȻΪ��ס״̬


				// ����̳߳�ֹͣ�����������Ϊ�գ���������
				if (this->stop && this->tasks.empty()){
					return;
				}
				// ȡ�������������(ע��˴���std::move)
				task = std::move(this->tasks.front());
				// �Ӷ����Ƴ�
				this->tasks.pop();
			}
			// ִ������
			task();
		}
	}
	);
}

// add new work item to the pool
// ���һ���µĹ��������̳߳�
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	// ����������������󶨣�����һ��packaged_task
	auto task = std::make_shared< std::packaged_task<return_type()> >(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);
	// ��ȡ�����future
	std::future<return_type> res = task->get_future();
	{
		// ��ռ����
		std::unique_lock<std::mutex> lock(queue_mutex);

		// don't allow enqueueing after stopping the pool
		// ��������ӵ��Ѿ�ֹͣ���̳߳�
		if (stop){
			throw std::runtime_error("enqueue on stopped ThreadPool");
		}
		// ��������ӵ��������
		tasks.emplace([task](){ (*task)(); });
	}
	// ����֪ͨ������ĳһ�������߳�ȡִ������
	condition.notify_one();
	return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
	{
		// ����
		std::unique_lock<std::mutex> lock(queue_mutex);
		// ֹͣ��־��true
		stop = true;
	}
	// ֪ͨ���й����̣߳����Ѻ���ΪstopΪtrue�ˣ����Զ������
	condition.notify_all();
	// �ȴ����й����߳̽���
	for (std::thread &worker : workers){
		worker.join();
	}
}

#endif