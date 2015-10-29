#include <utility>

#include <vector>
//#include <functional>
#include <map>
#include <string>
#include <boost\lexical_cast.hpp>
#include "function_traits.hpp"
class router
{
	class token_parser
	{
		std::vector<std::string> v_;
		static std::vector<std::string> split(std::string& s, char seperator)
		{
			std::vector<std::string> v;
			int pos = 0;
			while (true)
			{
				pos = s.find(seperator, 0);
				if (pos == std::string::npos)
				{
					if (!s.empty())
						v.push_back(s);
					break;
				}
				if (pos != 0)
					v.push_back(s.substr(0, pos));
				s = s.substr(pos + 1, s.length());
			}

			return v;
		}
	public:

		token_parser(std::string& s, char seperator)
		{
			v_ = split(s, seperator);
		}

	public:
		template<typename RequestedType>
		typename std::decay<RequestedType>::type get()
		{
			if (v_.empty())
				throw std::invalid_argument("unexpected end of input");

			try
			{
				typedef typename std::decay<RequestedType>::type result_type;

				auto it = v_.begin();
				result_type result = boost::lexical_cast<typename std::decay<result_type>::type>(*it);
				v_.erase(it);
				return result;
			}
			catch (std::exception& e)
			{
				throw std::invalid_argument(std::string("invalid argument: ") + e.what());
			}
		}

		bool empty() const{ return v_.empty(); }
	};

	typedef std::function<void(token_parser &)> invoker_function;

	std::map<std::string, invoker_function> map_invokers_;

public:
	template<typename Function>
	void register_handler(std::string const & name, const Function& f) {
		return register_nonmember_func(name, f);
	}

	void remove_handler(std::string const& name) {
		this->map_invokers_.erase(name);
	}

	void route(std::string  & text) const
	{
		token_parser parser(text, '/');

		while (!parser.empty())
		{
			std::string func_name = parser.get<std::string>();

			auto it = map_invokers_.find(func_name);
			if (it == map_invokers_.end())
				throw std::runtime_error("unknown function: " + func_name);

			//找到的function中，开始将字符串转换为函数实参并调用
			it->second(parser);
		}
	}


private:
	//将注册的handler保存在map中
	template<typename Function>
	void register_nonmember_func(std::string const & name, const Function& f)
	{
		this->map_invokers_[name] = std::bind(&invoker<Function>::template apply<std::tuple<>>, f,
			std::placeholders::_1, std::tuple<>());
	}

	//template<typename Function, class Signature = Function, size_t N = 0, size_t M = function_traits<Signature>::arity>
	//struct invoker;

	//遍历function的实参类型，将字符串参数转换为实参并添加到tuple中
	template<typename Function, size_t N = 0, size_t M = function_traits<Function>::arity>
	struct invoker
	{
		template<typename Args>
		static inline void apply(const Function& func, token_parser & parser, Args const & args)
		{
			typedef typename function_traits<Function>::template args<N>::type arg_type;
			router::invoker<Function, N + 1, M>::apply(func, parser, 
				std::tuple_cat(args, std::make_tuple(parser.get<arg_type>())));
		}
	};

	template<typename Function, size_t M>
	struct invoker<Function, M, M>
	{	
		template<typename Args>
		static inline void apply(const Function& func, token_parser &, Args const & args)
		{
			//参数列表已经准备好，可以调用function了
			call(func, args);
		}
	};
	
	template<int...>
	struct IndexTuple{};

	template<int N, int... Indexes>
	struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexes...>{};

	template<int... indexes>
	struct MakeIndexes<0, indexes...>
	{
		typedef IndexTuple<indexes...> type;
	};

	//C++14的实现
	//template<typename F, size_t... I, typename ... Args>
	//static void call_helper(F f, std::index_sequence<I...>, const std::tuple<Args...>& tup)
	//{
	//	f(std::get<I>(tup)...);
	//}

	//template<typename F, typename ... Args>
	//static void call(F f, const std::tuple<Args...>& tp)
	//{
	//	call_helper(f, std::make_index_sequence<sizeof... (Args)>(), tp);
	//}

	template<typename F, int ... Indexes, typename ... Args>
	static void call_helper(F f, IndexTuple<Indexes...>, const std::tuple<Args...>& tup)
	{
		f(std::get<Indexes>(tup)...);
	}

	template<typename F, typename ... Args>
	static void call(F f, const std::tuple<Args...>& tp)
	{
		call_helper(f, typename MakeIndexes<sizeof... (Args)>::type(), tp);
	}
};

/*
void add(int a, int b)
{
	std::cout << a + b << std::endl;
}

void hello()
{
	std::cout << "hello" << std::endl;
}

void foo(std::string b, int a)
{
	std::cout << b << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;

	router r;
	//设置路由
	r.register_handler("add", &add);
	r.register_handler("about", &hello);
	r.register_handler("fun", &foo);

	try
	{
		//解析uri实现调用
		string s1 = "add/1/2";
		string s2 = "about";
		string s3 = "fun/test/1";
		r.route(s1);
		r.route(s2);
		r.route(s3);
	}
	catch (std::runtime_error &error)
	{
		std::cerr << error.what() << std::endl;
	}

	return 0;
}
*/
