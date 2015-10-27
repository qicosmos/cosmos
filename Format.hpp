#pragma once
#include <tuple>
#include <type_traits>
#include <string>
#include <inttypes.h>
#include <cctype>

using namespace std;

namespace detail
{
    char g_buf[2000] = {};
    inline void FormatArg(char*& buf, int i)
    {
        _itoa(i, buf, 10);
        buf += strlen(buf);
    }

    inline void FormatArg(char*& buf, double i)
    {
        auto r = sprintf(buf, "%.6f", i);
        buf += r;
    }

    inline void FormatArg(char*& buf, int64_t i)
    {
        auto r = sprintf(buf, "%"PRId64, i);
        buf += r;
    }

    inline void FormatArg(char*& buf, uint64_t i)
    {
        auto r = sprintf(buf, "%"PRIu64, i);
        buf += r;
    }

    inline void FormatArg(char*& buf, const char* p)
    {
        int len = strlen(p);
        memcpy(buf, p, len);
        buf += len;
    }

    inline void FormatArg(char*& buf, string& s)
    {
        memcpy(buf, s.data(), s.size());
        buf += s.size();
    }

    template<size_t k, typename Tuple>
    typename std::enable_if < (k == std::tuple_size<Tuple>::value)>::type
    inline GetArgByIndex(size_t, Tuple&, char*&)
    {
        throw std::invalid_argument("arg index out of range");
    }

    template<size_t k = 0, typename Tuple>
    typename std::enable_if < (k < std::tuple_size<Tuple>::value)>::type
    inline GetArgByIndex(size_t index, Tuple& tp, char*& p)
    {
        if (k == index)
        {
            FormatArg(p, std::get<k>(tp));
        }
        else
        {
            GetArgByIndex<k + 1>(index, tp, p);
        }
    }

    inline int GetIndex(char*& p)
    {
        char temp[3] = {};
        int i = 0;
        while (*p != '}'&&*p != '\0')
        {
            if (i >= 2)
                throw std::invalid_argument("index is out of range.");

            if (std::isdigit(*p))
            {
                //push digit
                temp[i++] = *p;
                char next = *(p + 1);
                if (std::isdigit(next))
                {
                    temp[i++] = next;
                    p += 2;
                    continue;
                }

                //validate arg
                if (!std::isspace(next) && next != '}')
                {
                    throw std::invalid_argument("invalid argument.");
                }
            }

            p++;
        }

        return i == 0 ? -1 : std::atoi(temp);
    }
}

template<typename... Args>
inline string format(const string& src, Args... args)
{
    return format((char*) src.c_str(), args...);
}

template<typename... Args>
inline string format(char* src, Args... args)
{
    using namespace detail;
    char* buf = g_buf;
    auto tp = std::tuple<Args...>(args...);

    char* p = src;
    char* original = p;
    int len = strlen(src) + 1;
    int last = 0;
　　while (true)
    {
        if (*p == '{')
        {
            //copy content befor {
            last = p - original;
            memcpy(buf, original, last);
            buf += last;

            //format args
            int index = GetIndex(p);
            if (index >= 0)
            {
                GetArgByIndex<0>(index, tp, Fun, buf);
            }

            //skip }
            original = p + 1;
        }
　　　　　else if(*p=='\0')
        {　　　　　　　　

　　　　　　last = p - original;

　　　　　　memcpy(buf, original, last);
            break;
　　　　　}　　

        p++;
    }
　　string s = g_buf; 
　　memset(g_buf, 0, buf - g_buf); 
　　return s; 
}
