#pragma once
#include <memory>
#include <vector>
#include <functional>

template <class T>
class SimpleObjectPool
{
public:
    using DeleterType = std::function<void(T*)>;

    void add(std::unique_ptr<T> t)
    {
        pool_.push_back(std::move(t));
    }

    std::unique_ptr<T, DeleterType> get()
    {
        if (pool_.empty())
        {
            throw std::logic_error("no more object");
        }

        //every time add custom deleter for default unique_ptr
        std::unique_ptr<T, DeleterType> ptr(pool_.back().release(), [this](T* t)
        {
            pool_.push_back(std::unique_ptr<T>(t));
        });

        pool_.pop_back();
        return std::move(ptr);
    }
    
    std::shared_ptr<T> get_shared()
    {
        if (pool_.empty())
        {
            throw std::logic_error("no more object");
        }

        auto pin = std::unique_ptr<T>(std::move(pool_.back()));
        pool_.pop_back();

        return std::shared_ptr<T>(pin.release(), [this](T* t)   
        {  
            pool_.push_back(std::unique_ptr<T>(t));  
        }); 
    }

    bool empty() const
    {
        return pool_.empty();
    }

    size_t size() const
    {
        return pool_.size();
    }

private:
    std::vector<std::unique_ptr<T>> pool_;
};

//test code
void test_object_pool()
{
    SimpleObjectPool<A> p;
    p.add(std::unique_ptr<A>(new A()));
    p.add(std::unique_ptr<A>(new A()));
    {
        auto t = p.get();
        p.get();
    }

    {
        p.get();
        p.get();
    }

    std::cout << p.size() << std::endl;
｝

//for shared_ptr
std::shared_ptr<T> get()
{
	if (pool_.empty())
	{
		throw std::logic_error("no more object");
	}

	std::shared_ptr<T> ptr = pool_.back();
	auto p = std::shared_ptr<T>(new T(std::move(*ptr.get())), [this](T* t) 
	{
		pool_.push_back(std::shared_ptr<T>(t));
	});

	//std::unique_ptr<T, DeleterType> ptr(pool_.back().release(), [this](T* t)
	//{
	//	pool_.push_back(std::unique_ptr<T>(t));
	//});

	pool_.pop_back();
	return p;
}
