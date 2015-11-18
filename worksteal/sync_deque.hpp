#pragma once
#include <deque>
#include <mutex>

template <class T>
class sync_deque final
{
public:
	sync_deque() = default;
	~sync_deque() = default;

	//LIFO
	void push_front(const T &val)
	{
		std::unique_lock<std::mutex> l(mtx_);
		deque_.push_front(val);
	}

	T pop_front()
	{
		mtx_.lock();

		if (deque_.empty()) 
		{
			mtx_.unlock();
			return nullptr;
		}

		T val = deque_.front();
		deque_.pop_front();

		mtx_.unlock();

		return val;
	}

	//for steal FIFO
	T pop_back()
	{
		if (!mtx_.try_lock())
			return nullptr;

		if (deque_.empty()) 
		{
			mtx_.unlock();
			return nullptr;
		}

		T val = deque_.back();
		deque_.pop_back();

		mtx_.unlock();

		return val;
	}

	bool empty()
	{
		//std::unique_lock<std::mutex> l(mtx_);
		return deque_.empty();
	}

private:
	std::deque<T> deque_;
	std::mutex mtx_;
};

