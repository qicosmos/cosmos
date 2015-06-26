#ifndef _SCOPEGUARD_HPP_
#define _SCOPEGUARD_HPP_

#include <type_traits>

template <typename F>
class ScopeGuard
{
public:
	explicit ScopeGuard( F && f) : m_func(std::move(f)), m_dismiss(false){}
	explicit ScopeGuard(const F& f) : m_func(f), m_dismiss(false){}

	~ScopeGuard()
	{
		if (!m_dismiss)
			m_func();
	}

	ScopeGuard(ScopeGuard && rhs) : m_func(std::move(rhs.m_func)),
		m_dismiss(rhs.m_dismiss)
	{
		rhs.Dismiss();
	}

	void Dismiss()
	{
		m_dismiss = true;
	}

private:
	F m_func;
	bool m_dismiss;

	ScopeGuard();
	ScopeGuard(const ScopeGuard&);
	ScopeGuard& operator=(const ScopeGuard&);
};


template <typename F>
ScopeGuard<typename std::decay<F>::type> MakeGuard(F && f)
{
	return ScopeGuard<typename std::decay<F>::type>(std::forward<F>(f));
}


#endif  //_SCOPEGUARD_HPP_