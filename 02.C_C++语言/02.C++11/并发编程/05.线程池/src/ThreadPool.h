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

// 线程池类
class ThreadPool {
public:
	// 构造函数，传入线程数
	ThreadPool(size_t threads);
	// 入队任务(传入函数和函数的参数)
	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
		->std::future<typename std::result_of<F(Args...)>::type>;
	// 一个最简单的函数包装模板可以这样写(C++11)适用于任何函数(变参、成员都可以)
	// template<class F, class... Args>
	// auto enqueue(F&& f, Args&&... args) -> decltype(declval<F>()(declval<Args>()...))
	// {    return f(args...); }
	// C++14更简单
	// template<class F, class... Args>
	// auto enqueue(F&& f, Args&&... args)
	// {    return f(args...); }

	// 析构
	~ThreadPool();
private:
	// need to keep track of threads so we can join them
	// 工作线程组
	std::vector< std::thread > workers;
	// 任务队列
	std::queue< std::function<void()> > tasks;

	// synchronization 异步
	std::mutex queue_mutex; // 队列互斥锁
	std::condition_variable condition;  // 条件变量
	bool stop;  // 停止标志
};

// the constructor just launches some amount of workers
// 构造函数仅启动一些工作线程
inline ThreadPool::ThreadPool(size_t threads)
: stop(false)
{
	for (size_t i = 0; i<threads; ++i)
		// 添加线程到工作线程组
		workers.emplace_back(   // 与push_back类型，但性能更好(与此类似的还有emplace/emlace_front)
		[this]
	{   // 线程内不断的从任务队列取任务执行
		for (;;)
		{
			std::function<void()> task;

			{
				// 拿锁(独占所有权式)
				std::unique_lock<std::mutex> lock(this->queue_mutex);
				// 等待条件成立
				this->condition.wait(lock,
					[this]{ return this->stop || !this->tasks.empty(); });
				// 执行条件变量等待的时候，已经拿到了锁(即lock已经拿到锁，没有阻塞)
				// 这里将会unlock释放锁，其他线程可以继续拿锁，但此处任然阻塞，等待条件成立
				// 一旦收到其他线程notify_*唤醒，则再次lock，然后进行条件判断
				// 当[return this->stop || !this->tasks.empty()]的结果为false将阻塞
				// 条件为true时候解除阻塞。此时lock依然为锁住状态


				// 如果线程池停止或者任务队列为空，结束返回
				if (this->stop && this->tasks.empty()){
					return;
				}
				// 取得任务队首任务(注意此处的std::move)
				task = std::move(this->tasks.front());
				// 从队列移除
				this->tasks.pop();
			}
			// 执行任务
			task();
		}
	}
	);
}

// add new work item to the pool
// 添加一个新的工作任务到线程池
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	// 将任务函数和其参数绑定，构建一个packaged_task
	auto task = std::make_shared< std::packaged_task<return_type()> >(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);
	// 获取任务的future
	std::future<return_type> res = task->get_future();
	{
		// 独占拿锁
		std::unique_lock<std::mutex> lock(queue_mutex);

		// don't allow enqueueing after stopping the pool
		// 不允许入队到已经停止的线程池
		if (stop){
			throw std::runtime_error("enqueue on stopped ThreadPool");
		}
		// 将任务添加到任务队列
		tasks.emplace([task](){ (*task)(); });
	}
	// 发送通知，唤醒某一个工作线程取执行任务
	condition.notify_one();
	return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
	{
		// 拿锁
		std::unique_lock<std::mutex> lock(queue_mutex);
		// 停止标志置true
		stop = true;
	}
	// 通知所有工作线程，唤醒后因为stop为true了，所以都会结束
	condition.notify_all();
	// 等待所有工作线程结束
	for (std::thread &worker : workers){
		worker.join();
	}
}

#endif