#pragma once
#include <map>
#include <string>
#include <functional>
#include <memory>
#include "Message.hpp"

struct factory
{
	template<typename T>
	struct register_t
	{
		register_t(const std::string& key)
		{
			factory::get().map_.emplace(key, &register_t<T>::create);
		}

		template<typename... Args>
		register_t(const std::string& key, Args... args)
		{
			factory::get().map_.emplace(key, [=] { return new T(args...); });
		}
		inline static Message* create() { return new T; }
	};

	inline Message* produce(const std::string& key)
	{
		if (map_.find(key) == map_.end())
			throw std::invalid_argument("the message key is not exist!");

		return map_[key]();
	}

	std::unique_ptr<Message> produce_unique(const std::string& key)
	{
		return std::unique_ptr<Message>(produce(key));
	}

	std::shared_ptr<Message> produce_shared(const std::string& key)
	{
		return std::shared_ptr<Message>(produce(key));
	}
	typedef Message*(*FunPtr)();

	inline static factory& get()
	{
		static factory instance;
		return instance;
	}
	
private:
	factory() {};
	factory(const factory&) = delete;
	factory(factory&&) = delete;
	
	std::map<std::string, FunPtr> map_;
};

//std::map<std::string, factory::FunPtr> factory::map_;

#define REGISTER_MESSAGE_VNAME(T) reg_msg_##T##_
#define REGISTER_MESSAGE(T, key, ...) static factory::register_t<T> REGISTER_MESSAGE_VNAME(T)(key, ##__VA_ARGS__);

/*
Message* p = factory::get().produce("message1");
p->foo();   //Message1
*/
