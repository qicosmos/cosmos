#pragma once
#include <thread>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include "sync_deque.hpp"

class worker_t;
using workers_ptr = std::shared_ptr<std::vector<std::shared_ptr<worker_t>>>;
using task_t = std::function<void()>;

class worker_t final
{
public:
	
	worker_t(workers_ptr workers, int work_num) : workers_(workers), work_num_(work_num), enable_(true)
	{
		thread_ = std::thread(&worker_t::execute, this);
	}

	~worker_t()
	{
	}

	void assign(const task_t& task)
	{
		queue_.push_front(task);
	}

	task_t steal()
	{
		return queue_.pop_back();
	}

	bool empty()
	{
		return queue_.empty();
	}

	void join()
	{
		enable_ = false;
		thread_.join();
	}

private:
	void execute()
	{
		thread_id_ = std::this_thread::get_id();
		while (enable_)
		{
			//not ready
			if (work_num_ != workers_->size())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			
			//do work
			task_t t = queue_.pop_front();
			while (t!=nullptr)
			{
				t();
				t = queue_.pop_front();
			}

			//check if has task
			bool no_task = std::all_of(workers_->begin(), workers_->end(), [](std::shared_ptr<worker_t> worker) {return worker->empty(); });
			if (no_task)
			{
                //std::cout<<"all tasks have been finished!"<<std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				continue;
			}

			//steal
			int rand_select = rand() % workers_->size();
			if (workers_->at(rand_select) == nullptr)
				continue;

			t = workers_->at(rand_select)->steal();

			if (t != nullptr)
			{
				t();
			}
		}
	}
	
	std::thread thread_;
	std::shared_ptr<std::vector<std::shared_ptr<worker_t>>> workers_;
	std::thread::id thread_id_;
	sync_deque<task_t> queue_;

	int work_num_;
	bool enable_;
};

