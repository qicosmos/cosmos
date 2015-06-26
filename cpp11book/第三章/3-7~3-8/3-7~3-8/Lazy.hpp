#ifndef _LAZY_HPP_
#define _LAZY_HPP_

#include <boost/optional.hpp>
template<typename T>
struct Lazy
{
	Lazy(){}

	template <typename Func, typename... Args>
	Lazy(Func& f, Args && ... args)
	{
		m_func = [&f, &args...]{return f(args...); };
	}

	T& Value()
	{
		if (!m_value.is_initialized())
		{
			m_value = m_func();
		}

		return *m_value;
	}

	bool IsValueCreated() const
	{
		return m_value.is_initialized();
	}

private:
	std::function<T()> m_func;
	boost::optional<T> m_value;
};

template<class Func, typename... Args>
Lazy<typename std::result_of<Func(Args...)>::type>
lazy(Func && fun, Args && ... args)
{
	return Lazy<typename std::result_of<Func(Args...)>::type>(
		std::forward<Func>(fun), std::forward<Args>(args)...);
}


#endif