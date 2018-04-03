#include "dispatchQueue.h"

dispatch_queue::dispatch_queue(size_t thread_cnt) : threads(thread_cnt)
{
	for (size_t i = 0; i < threads.size(); i++)
	{
		threads[i] = std::thread(
			std::bind(&dispatch_queue::dispatch_thread_handler, this));
	}
}

dispatch_queue::~dispatch_queue()
{
	// Signal to dispatch threads that it's time to wrap up
	quit = true;
	cv.notify_all();

	// Wait for threads to finish before we exit
	for (size_t i = 0; i < threads.size(); i++)
	{
		if (threads[i].joinable())
		{
			threads[i].join();
		}
	}
}

void dispatch_queue::activate_completion() {
	completion_active = true;
	completion_flag = false;
	threadCount = 0;
}

void dispatch_queue::dispatch(const fp_t& op)
{
	std::unique_lock<std::mutex> lock(queue_lock);

	if (completion_active) {
		threadCount++;
		completion_flag = false;
	}
	
	q.push(op);

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lock.unlock();
	cv.notify_one();
}

void dispatch_queue::dispatch(fp_t&& op)
{
	std::unique_lock<std::mutex> lock(queue_lock);

	if (completion_active) {
		threadCount++;
		completion_flag = false;
	}

	q.push(std::move(op));

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lock.unlock();
	cv.notify_one();
}


void dispatch_queue::dispatch_thread_handler(void)
{
	std::unique_lock<std::mutex> lock(queue_lock);

	do {
		//Wait until we have data or a quit signal
		cv.wait(lock, [this] {
			return (q.size() || quit);
		});

		//after wait, we own the lock
		if (q.size() && !quit)
		{
			auto op = std::move(q.front());
			q.pop();

			//unlock now that we're done messing with the queue
			lock.unlock();

			op();

			lock.lock();

			if (completion_active) {

				if (--threadCount == 0) {

					std::unique_lock<std::mutex> c_lock(completion_lock);
					completion_flag = true;
					c_lock.unlock();
					completion_cv.notify_all();
				}

			}
		}
	} while (!quit);
}

void dispatch_queue::wait_complete()
{
	std::unique_lock<std::mutex> lock(completion_lock);
	if (!completion_flag) {
		completion_cv.wait(lock, [this] {
			return (completion_flag.load());
		});
	}
}