#pragma once

namespace XV {

#if defined(_MSC_VER)
#define FORCEINLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3
#define FORCEINLINE inline __attribute__((__always_inline__))
#else
#define FORCEINLINE inline
#endif

#define PAD_RANDOM(num) char pad##num[(__COUNTER__ % 5 + 1)]{};

#pragma pack( push, 1 )
    template<typename T>
    class XORVar {
    private:
        PAD_RANDOM(1)
            T m_value;
        PAD_RANDOM(2)
            unsigned char m_length;
        PAD_RANDOM(3)
            T m_key;
        PAD_RANDOM(4)
            T m_key2;
        PAD_RANDOM(5)

            FORCEINLINE void SetValue(T newValue) {
            T result = newValue;
            for (int i = 0; i < m_length; ++i) {
                *(reinterpret_cast<unsigned char*>(&result) + i) ^= i % 2 ? ~*reinterpret_cast<unsigned char*>(&m_key + i) : ~*reinterpret_cast<unsigned char*>(&m_key2 + (m_length - i));
            }
            m_value = result;
        }

        FORCEINLINE T GetValue() {
            T result = m_value;
            for (int i = 0; i < m_length; ++i) {
                *(reinterpret_cast<unsigned char*>(&result) + i) ^= i % 2 ? ~*reinterpret_cast<unsigned char*>(&m_key + i) : ~*reinterpret_cast<unsigned char*>(&m_key2 + (m_length - i));
            }
            return result;
        }

    public:
        
        FORCEINLINE XORVar() {
            m_value = {};
            m_key = {};
            m_key2 = {};
            m_length = sizeof(T);

            for (auto i = 0u; i < sizeof(T); i++) {
                *(reinterpret_cast<unsigned char*>(&m_key) + i) = (unsigned char)(__rdtsc() & 0xFF);
            }
            for (auto i = 0u; i < sizeof(T); i++) {
                *(reinterpret_cast<unsigned char*>(&m_key2) + i) = (unsigned char)(__rdtsc() & 0xFF) ^ ~*reinterpret_cast<unsigned char*>(&m_key + i);
            }
        }

        FORCEINLINE XORVar(T newValue) {
            m_value = {};
            m_key = {};
            m_key2 = {};
            m_length = sizeof(T);

            for (auto i = 0u; i < sizeof(T); i++) {
                *(reinterpret_cast<unsigned char*>(&m_key) + i) = (unsigned char)(__rdtsc() & 0xFF);
            }
            for (auto i = 0u; i < sizeof(T); i++) {
                *(reinterpret_cast<unsigned char*>(&m_key2) + i) = (unsigned char)(__rdtsc() & 0xFF) ^ ~*reinterpret_cast<unsigned char*>(&m_key + i);
            }

            SetValue(newValue);
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
            return m_address()(std::forward<Args>(args)...);
        }
    };
#pragma pack( pop )

}