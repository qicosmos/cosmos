#pragma once
#include <cstddef>
#include <assert.h>
#include <cstdint>
using namespace std;

enum TypeTag {
    BOOL,
    INT32,
    UINT32,
    UINT64,
    INT64,
    DOUBLE,
    CHAR_ARRAY,
    STRING,
    NIL
};

#define PAYLOAD_MASK 0x00007FFFFFFFFFFFULL
#define NAN_MASK 0x7FF8000000000000ULL
#define TAG_MASK 0xF
#define TAG_SHIFT 47

union Value {
    uint64_t ival;
    double fval;

    Value(double x) : fval(x)
    {
    }

    Value(int v)
    {
        ival = NAN_MASK | ((uint64_t)INT32 << TAG_SHIFT) | (uint64_t)v;
    }
    Value(uint32_t v)
    {
        ival = NAN_MASK | ((uint64_t)UINT32 << TAG_SHIFT) | (uint64_t)v;
    }

    Value(int64_t v)
    {
        ival = static_cast<uint64_t>(v);
    }

    Value(uint64_t v)
    {
        ival = v;
    }

    Value(bool v)
    {
        ival = NAN_MASK | ((uint64_t)BOOL << TAG_SHIFT) | (uint64_t)v;
    }

    Value(const char* v)
    {
        ival = NAN_MASK | ((uint64_t)CHAR_ARRAY << TAG_SHIFT) | (uint64_t)v;
    }

    Value(const string& v)
    {
        ival = NAN_MASK | ((uint64_t)STRING << TAG_SHIFT) | (uint64_t)&v;
    }

    Value(TypeTag tag = NIL, void *payload = nullptr) {
        assert((uint64_t)payload <= PAYLOAD_MASK);
        ival = NAN_MASK | ((uint64_t)tag << TAG_SHIFT) | (uint64_t)payload;
    }

    int toInt() const
    {
        assert(getTag() == INT32);
        return (int)getPayload();
    }

    int64_t toInt64() const
    {
        return (int64_t)ival;
    }

    uint32_t toUInt() const
    {
        assert(getTag() == UINT32);
        return (int)getPayload();
    }

    uint64_t toUInt64() const
    {
        return ival;
    }

    bool toBool() const
    {
        assert(getTag() == BOOL);
        return getPayload() != 0;
    }

    double toDouble() const
    {
        assert(getTag() == DOUBLE);
        return fval;
    }

    char *toCharArray() const
    {
        assert(getTag() == CHAR_ARRAY);
        return (char *)getPayload();
    }

    string& toString() const
    {
        assert(getTag() == STRING);
        return *(string *)getPayload();
    }

    TypeTag getTag() const
    {
        return isPayload() ? DOUBLE : TypeTag((ival >> TAG_SHIFT) & TAG_MASK);
    }

    uint64_t getPayload() const
    {
        assert(!isPayload());
        return ival & PAYLOAD_MASK;
    }

    bool operator<(const Value& other)  const
    {
        return hash()<other.hash();
    }

    bool operator==(const Value& other)  const
    {
        return hash() == other.hash();
    }
private:
    bool isPayload() const
    {
        return (int64_t)ival <= (int64_t)NAN_MASK;
    }

    uint64_t toHashUInt64() const
    {
        assert(getTag() < INT64);
        if (getTag() == UINT64)
            return ival;

        return (uint64_t)getPayload();
    }

    int64_t toHashInt64() const
    {
        assert(getTag() == INT64);
        return toInt64();
    }

    std::size_t hash() const {
        switch (getTag()) {
        case         UINT64:
        case         INT32:
        case         UINT32:
        case           BOOL:
            return std::hash<uint64_t>()(toHashUInt64());
        case         INT64:
            return std::hash<int64_t>()(toHashInt64());
        case DOUBLE:
            return std::hash<double>()(toDouble());
        case STRING:
            return std::hash<std::string>()(toString());
        default:
            throw std::invalid_argument("can not find this type");
        }
    }
};
