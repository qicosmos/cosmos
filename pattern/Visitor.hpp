template<typename... Types>
struct Visitor;

template<typename T, typename... Types>
struct Visitor<T, Types...> : Visitor<Types...>
{
    using Visitor<Types...>::Visit;//通过using避免隐藏基类的visit同名方法
    virtual void Visit(const T&) = 0;
};

template<typename T>
struct Visitor<T>
{
    virtual void Visit(const T&) = 0;
};
