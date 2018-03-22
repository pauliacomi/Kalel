#include "dispatch_queue.h"

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

void dispatch_queue::dispatch(const fp_t& op)
{
	std::unique_lock<std::mutex> lock(lock);
	q.push(op);

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lock.unlock();
	cv.notify_all();
}

void dispatch_queue::dispatch(fp_t&& op)
{
	std::unique_lock<std::mutex> lock(lock);
	q.push(std::move(op));

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lock.unlock();
	cv.notify_all();
}

void dispatch_queue::dispatch_thread_handler(void)
{
	std::unique_lock<std::mutex> lock(lock);

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
		}
	} while (!quit);
}