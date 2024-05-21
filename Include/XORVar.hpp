#pragma once
#include <intrin.h>

namespace XV {

#if defined(_MSC_VER)
#define FORCEINLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3
#define FORCEINLINE inline __attribute__((__always_inline__))
#else
#define FORCEINLINE inline
#endif

#define CONV_STR2DEC_1(str, i)  (str[i]>'0'?str[i]-'0':0)
#define CONV_STR2DEC_2(str, i)  (CONV_STR2DEC_1(str, i)*10 + str[i+1]-'0')
#define __TIME_SECONDS__        CONV_STR2DEC_2(__TIME__, 6)

#define PAD_RANDOM(num) char pad##num[((__TIME_SECONDS__ + __COUNTER__) % 8) + 4]{};

#define FILL_PAD(num)   for (auto i = 0; i < sizeof(pad##num); i++) { \
                            pad##num[i] = (unsigned char)(__rdtsc() & 0xFF); \
                        }

#pragma pack( push, 1 )
    template<typename T>
    class XORVar {
    private:
        PAD_RANDOM(1)
            T m_value;
        PAD_RANDOM(2)
            unsigned char m_length;
        PAD_RANDOM(3)
            unsigned long m_key;
        PAD_RANDOM(4)
            unsigned long m_key2;
        PAD_RANDOM(5)

    public:

        FORCEINLINE XORVar() {
            m_value = {};
            m_key = {};
            m_key2 = {};
            m_length = sizeof(T);

            FILL_PAD(1)
            FILL_PAD(2)
            FILL_PAD(3)
            FILL_PAD(4)
            FILL_PAD(5)
             
            for (auto i = 0u; i < sizeof(m_key); i++) {
                *(reinterpret_cast<unsigned char*>(&m_key) + i) = (unsigned char)(__rdtsc() & 0xFF);
            }
            for (auto i = 0u; i < sizeof(m_key2); i++) {
                *(reinterpret_cast<unsigned char*>(&m_key2) + i) = (unsigned char)(__rdtsc() & 0xFF) ^ ~*reinterpret_cast<unsigned char*>(&m_key + i);
            }
        }

        FORCEINLINE XORVar(T newValue) {
            m_value = {};
            m_key = {};
            m_key2 = {};
            m_length = sizeof(T);

            FILL_PAD(1)
            FILL_PAD(2)
            FILL_PAD(3)
            FILL_PAD(4)
            FILL_PAD(5)

            for (auto i = 0u; i < sizeof(m_key); ++i) {
                *(reinterpret_cast<unsigned char*>(&m_key) + i) = (unsigned char)(__rdtsc() & 0xFF);
            }
            for (auto i = 0u; i < sizeof(m_key2); ++i) {
                *(reinterpret_cast<unsigned char*>(&m_key2) + i) = (unsigned char)(__rdtsc() & 0xFF) ^ ~*reinterpret_cast<unsigned char*>(&m_key + i);
            }

            SetValue(newValue);
        }

        FORCEINLINE void SetValue(T newValue) {
            T result = newValue;
            for (int i = 0; i < m_length; i++) {
                *(reinterpret_cast<unsigned char*>(&result) + i) ^= i % 2 ? ~*reinterpret_cast<unsigned char*>(&m_key + (i % sizeof(m_key))) : ~*reinterpret_cast<unsigned char*>(&m_key2 + (i % sizeof(m_key2)));
            }
            m_value = result;
        }

        FORCEINLINE T GetValue() {
            T result = m_value;
            for (int i = 0; i < m_length; i++) {
                *(reinterpret_cast<unsigned char*>(&result) + i) ^= i % 2 ? ~*reinterpret_cast<unsigned char*>(&m_key + (i % sizeof(m_key))) : ~*reinterpret_cast<unsigned char*>(&m_key2 + (i % sizeof(m_key2)));
            }
            return result;
        }

        FORCEINLINE void operator=(T newValue) {
            SetValue(newValue);
        }

        FORCEINLINE void operator=(XORVar<T>& newValue) {
            *this = newValue;
        }

        FORCEINLINE T operator()() {
            return GetValue();
        }

        FORCEINLINE XORVar<T>& operator++()
        {
            const auto value = GetValue()++;
            SetValue(value);
            return *this;
        }

        FORCEINLINE XORVar<T>& operator--()
        {
            const auto value = GetValue()--;
            SetValue(value);
            return *this;
        }

        FORCEINLINE XORVar<T>& operator*=(const T v) {
            const auto value = GetValue() * v;
            SetValue(value);
            return *this;
        }

        FORCEINLINE XORVar<T>& operator*=(const XORVar<T>& v) {
            const auto value = GetValue() * v.GetValue();
            SetValue(value);
            return *this;
        }

        FORCEINLINE XORVar<T>& operator/=(const T v) {
            const auto value = GetValue() / v;
            SetValue(value);
            return *this;
        }

        FORCEINLINE XORVar<T>& operator/=(const XORVar<T>& v) {
            const auto value = GetValue() / v.GetValue();
            SetValue(value);
            return *this;
        }

        FORCEINLINE XORVar<T>& operator+=(const T v) {
            const auto value = GetValue() + v;
            SetValue(value);
            return *this;
        }

        FORCEINLINE XORVar<T>& operator+=(const XORVar<T>& v) {
            const auto value = GetValue() + v.GetValue();
            SetValue(value);
            return *this;
        }

        FORCEINLINE  XORVar<T>& operator-=(const T v) {
            const auto value = GetValue() - v;
            SetValue(value);
            return *this;
        }

        FORCEINLINE XORVar<T>& operator-=(const XORVar<T>& v) {
            const auto value = GetValue() - v.GetValue();
            SetValue(value);
            return *this;
        }

        FORCEINLINE T operator-(const T v) {
            return GetValue() - v;
        }

        FORCEINLINE T operator-(const XORVar<T>& v) {
            return GetValue() - v.GetValue();
        }

        FORCEINLINE T operator+(const T v) {
            return GetValue() + v;
        }

        FORCEINLINE T operator+(const XORVar<T>& v) {
            return GetValue() + v.GetValue();
        }

        FORCEINLINE T operator/(const T v) {
            return GetValue() / v;
        }

        FORCEINLINE T operator/(const XORVar<T>& v) {
            return GetValue() / v.GetValue();
        }

        FORCEINLINE T operator*(const T v) {
            return GetValue() * v;
        }

        FORCEINLINE T operator*(const XORVar<T>& v) {
            return GetValue() * v.GetValue();
        }

        FORCEINLINE T operator->() {
            if (std::is_pointer<T>::value) {
                return GetValue();
            }
        }

        FORCEINLINE bool operator< (const XORVar<T>& rhs) { return rhs.GetValue() > this->GetValue(); }
        FORCEINLINE bool operator> (const XORVar<T>& rhs) { return rhs < *this; }
        FORCEINLINE bool operator<=(const XORVar<T>& rhs) { return !(*this > rhs); }
        FORCEINLINE bool operator>=(const XORVar<T>& rhs) { return !(*this < rhs); }

        FORCEINLINE bool operator< (const T& rhs) { return rhs > this->GetValue(); }
        FORCEINLINE bool operator> (const T& rhs) { return rhs < this->GetValue(); }
        FORCEINLINE bool operator<=(const T& rhs) { return this->GetValue() <= rhs; }
        FORCEINLINE bool operator>=(const T& rhs) { return this->GetValue() >= rhs; }

        FORCEINLINE bool operator==(const XORVar<T>& rhs) { return this->GetValue() == rhs.GetValue(); }
        FORCEINLINE bool operator==(const T& rhs) { return this->GetValue() == rhs; }
        FORCEINLINE bool operator!=(const XORVar<T>& rhs) { return this->GetValue() != rhs.GetValue(); }
        FORCEINLINE bool operator!=(const T& rhs) { return !(this->GetValue() == rhs); }
    };

    template<class T>
    class XORFunc {
    private:
        XORVar<T> m_address;

    public:
        FORCEINLINE XORFunc() {
            m_address = {};
        }

        FORCEINLINE XORFunc(T address) {
            m_address = address;
        }

        FORCEINLINE void SetAddress(T address) {
            m_address = address;
        }

        template<class... Args>
        FORCEINLINE decltype(auto) operator()(Args&&... args)
        {
            return m_address.GetValue()(args...);
        }
    };
#pragma pack( pop )

}

#define XORFUNC(name, function) XV::XORFunc<decltype(function)*> name##(function);


