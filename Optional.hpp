#pragma once
#include <type_traits>
#include <utility>
#include <stdexcept>

template<typename T>
class Optional
{
    using data_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;
public:
    Optional() : has_init_(false) {}

    Optional(const T& v)
    {
        create(v);
    }

    Optional(T&& v) : has_init_(false)
    {
        create(std::move(v));
    }

    ~Optional()
    {
        destroy();
    }

    Optional(const Optional& other) : has_init_(false)
    {
        if (other.isInit())
            assign(other);
    }

    Optional(Optional&& other) : has_init_(false)
    {
        if (other.isInit())
        {
            assign(std::move(other));
            other.destroy();
        }
    }

    Optional& operator=(Optional &&other)
    {
        assign(std::move(other));
        return *this;
    }

    Optional& operator=(const Optional &other)
    {
        assign(other);
        return *this;
    }

    template<class... Args>
    void emplace(Args&&... args)
    {
        destroy();
        create(std::forward<Args>(args)...);
    }

    bool isInit() const { return has_init_; }

    explicit operator bool() const 
    {
        return isInit();
    }

    T& operator*()
    {
        if(isInit())
        {
            return *((T*) (&data_));
        }

        throw std::logic_error{"try to get data in a Optional which is not initialized"};
    }

    const T& operator*() const
    {
        if(isInit())
        {
            return *((T*) (&data_));
        }

        throw std::logic_error{"try to get data in a Optional which is not initialized"};
    }

    T* operator->()
    {
        return &operator*();
    }

    const T* operator->() const
    {
        return &operator*();
    }

    bool operator==(const Optional<T>& rhs) const
    {
        return (!bool(*this)) != (!rhs) ? false : (!bool(*this) ? true : (*(*this)) == (*rhs));
    }

    bool operator<(const Optional<T>& rhs) const
    {
        return !rhs ? false : (!bool(*this) ? true : (*(*this) < (*rhs)));
    }

    bool operator!=(const Optional<T>& rhs)
    {
        return !(*this == (rhs));
    }
private:
    template<class... Args>
    void create(Args&&... args)
    {
        new (&data_) T(std::forward<Args>

            (args)...);
        has_init_ = true;
    }

    void destroy()
    {
        if (has_init_)
        {
            has_init_ = false;
            ((T*) (&data_))->~T();
        }
    }

    void assign(const Optional& other)
    {
        if (other.isInit())
        {
            copy(other.data_);
            has_init_ = true;
        }
        else
        {
            destroy();
        }
    }

    void assign(Optional&& other)
    {
        if (other.isInit())
        {
            move(std::move(other.data_));
            has_init_ = true;
            other.destroy();
        }
        else
        {
            destroy();
        }
    }

    void move(data_t&& val)
    {
        destroy();
        new (&data_) T(std::move(*((T*)(&val))));
    }

    void copy(const data_t& val)
    {
        destroy();
        new (&data_) T(*((T*) (&val)));
    }

private:
    bool has_init_;
    data_t data_;
};
