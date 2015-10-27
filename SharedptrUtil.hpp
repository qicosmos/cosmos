#pragma once
#include <memory>
using namespace std;

//通过shared_ptr创建数组
template<typename T> 
shared_ptr<T> make_shared_array(size_t size)
{
   return shared_ptr<T>(new T[size], default_delete<T[]>());
}

//make_unique的实现
//支持普通指针
template<class T, class... Args> inline
typename enable_if<!is_array<T>::value, unique_ptr<T> >::type make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

//支持动态数组
template<class T> inline
typename enable_if<is_array<T>::value && extent<T>::value==0, unique_ptr<T> >::type 

make_unique(size_t size)
{
    typedef typename remove_extent<T>::type U;
    return unique_ptr<T>(new U[size]());
}

//过滤掉定长数组的情况
template<class T, class... Args>
typename enable_if<extent<T>::value != 0, void>::type make_unique(Args&&...) = delete;

//------shared_ptr异常安全地管理第三方内存
#define GUARD(P) std::shared_ptr<void> p##p(p, [](void*p){ GetHandle()->Release(p);});

void* p = GetHandle()->Create();
GUARD(p); //安全

//------unique_ptr异常安全地管理第三方内存
#define GUARD(P) std::unique_ptr<void, void(*)(int*)> p##p(p, [](void*p){ GetHandle()->Release(p);});
