#ifndef _RANGE_HPP_
#define _RANGE_HPP_

namespace detail_range
{
	template<typename value_t>
	class RangeImpl
	{
		class Iterator;
	public:
		RangeImpl(value_t begin, value_t end, value_t step = 1) :m_begin(begin), m_end(end), m_step(step)
		{
			if (step > 0 && m_begin >= m_end)
				throw std::logic_error("end must greater than begin.");
			else if (step < 0 && m_begin <= m_end)
				throw std::logic_error("end must less than begin.");

			m_step_end = (m_end - m_begin) / m_step;
			if (m_begin + m_step_end*m_step != m_end)
			{
				m_step_end++;
			}
		}

		Iterator begin()
		{
			return Iterator(0, *this);
		}

		Iterator end()
		{
			return Iterator(m_step_end, *this);
		}

		value_t operator[](int s)
		{
			return m_begin + s*m_step;
		}

		int size()
		{
			return m_step_end;
		}

	private:
		value_t m_begin;
		value_t m_end;
		value_t m_step;
		int m_step_end;

		class Iterator
		{
		public:
			Iterator(int start, RangeImpl& range) : m_current_step(start), m_range(range)
			{
				m_current_value = m_range.m_begin + m_current_step*m_range.m_step;
			}

			value_t operator*() { return m_current_value; }

			const Iterator* operator++()
			{
				m_current_value += m_range.m_step;
				m_current_step++;
				return this;
			}

			bool operator==(const Iterator& other)
			{
				return m_current_step == other.m_current_step;
			}

			bool operator!=(const Iterator& other)
			{
				return m_current_step != other.m_current_step;
			}

			const Iterator* operator--()
			{
				m_current_value -= m_range.m_step;
				m_current_step--;
				return this;
			}

		private:
			value_t m_current_value;
			int m_current_step;
			RangeImpl& m_range;
		};
	};

	template<typename T, typename V>
	auto Range(T begin, T end, V stepsize)->RangeImpl<decltype(begin + end + stepsize)>
	{
		return RangeImpl<decltype(begin + end + stepsize)>(begin, end, stepsize);
	}

	template<typename T>
	RangeImpl<T> Range(T begin, T end)
	{
		return RangeImpl<T>(begin, end, 1);
	}

	template<typename T>
	RangeImpl<T> Range(T end)
	{
		return RangeImpl<T>(T(), end, 1);
	}
}


#endif  //_RANGE_HPP_