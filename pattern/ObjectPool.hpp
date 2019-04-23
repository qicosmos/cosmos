#include <string>
#include <functional>
#include <memory>
#include <map>
#include "NonCopyable.hpp"
using namespace std;

template <typename T>
class ObjectPool : NonCopyable {
	template <typename... Args>
	using Constructor = std::function<std::shared_ptr<T>(Args...)>;
public:
	struct ReEmplace {
		void operator() (T* p) {
			if (pool->isDestruct) {
				delete p;
			} else {
				ReEmplace newRe(this->pool, this->conName);
				pool->m_object_map.emplace(std::move(conName),
					std::shared_ptr<T>(p, newRe));
			}
		}
		ReEmplace(ObjectPool* p, const std::string& s) : pool(p), conName(s) {};
		ObjectPool* pool;
		std::string conName;
	};
	//默认创建多少个对象
	template <typename... Args>
	void Init(size_t num, Args&&... args) {
		if (num <= 0 || num > MaxObjectNum) {
			throw std::logic_error("object num out of range.");
		}
		auto constructName = typeid(Constructor<Args...>).name();//不区分引用
		//删除器中不直接删除对象，而是回收到对象池中，以供下次使用
		ReEmplace del(this, constructName);
		for (size_t i = 0; i < num; i++) {
			m_object_map.emplace(constructName, 
				std::shared_ptr<T>(new T(std::forward<Args>(args)...), del)); 
		}
	}
	//从对象池中获取一个对象
	template <typename... Args>
	std::shared_ptr<T> Get() {
		std::string constructName = typeid(Constructor<Args...>).name();
		auto range = m_object_map.equal_range(constructName);
		for (auto it = range.first; it != range.second; ++it) {
			auto ptr = it->second;
			m_object_map.erase(it);
			return ptr;
		}
		return nullptr;
	}
	~ObjectPool() {
		isDestruct = true;
	}
private:
	const size_t MaxObjectNum = 10;
	std::multimap<std::string, std::shared_ptr<T>> m_object_map;
	bool isDestruct = false;
};
