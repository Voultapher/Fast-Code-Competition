#ifndef FMT_HPP_INCLUDED
#define FMT_HPP_INCLUDED

#include <limits>
#include <cstdint>
#include <cstdint>
#include <tuple>
#include <array>
#include <string>

namespace fixed
{

// ---fixed_type---
template<
    size_t size_,
    typename C
> class fixed_type
{
public:
    using value_type = typename C::value_type;

    explicit fixed_type() noexcept
    {
        for (size_t i = 0; i < size_ - 1; ++i)
            buff_[i] = '\0';
        buff_.back() = '\n';
    }

    template<typename... Args> fixed_type(Args&&... args)
  	{
    	C{ buff_, std::forward<Args>(args)... };
    }

    fixed_type(const fixed_type&) = default;
    fixed_type(fixed_type&&) = default;

    fixed_type& operator= (const fixed_type&) = default;
    fixed_type& operator= (fixed_type&&) = default;

    ~fixed_type() = default;

    // would be nicer with dot operator
    constexpr operator value_type() const noexcept
    {
        return C::deserialize(buff_);
    }

    static constexpr size_t size() noexcept { return size_; }

    constexpr const char* data() const noexcept { return buff_.data(); }

private:
    std::array<char, size_> buff_;
};

// ---fixed_wrapper---
template<typename C> struct wrapper : C
{
    static_assert(std::alignment_of<C>::value == sizeof(char),
        "User class has to be byte aligned!");

    using C::C;

    const char* data() const noexcept
    {
        return reinterpret_cast<const char*>(this);
    }

    size_t size() const noexcept
    {
        return sizeof(*this);
    }
};


// ---fixed_basic_string---
namespace detail
{
struct fixed_basic_string
{
    using value_type = char*;

    template<size_t b_size, size_t c_size> fixed_basic_string(
        std::array<char, b_size>& arr,
        const char (&buff)[c_size]
    )
    {
        static_assert(c_size < b_size,
            "String too large for fixed_basic_string");

        for (size_t i = 0; i < c_size; ++i)
            arr[i] = buff[i];

        for (size_t i = c_size; i < b_size; ++i)
              arr[i] = '\0';

        arr.back() = '\n';
    }

    template<size_t size> static value_type deserialize(
        const std::array<char, size>& arr
    )
    {
        return const_cast<char*>(arr.data());
    }
};
} // namespace detail

template<size_t size> using fixed_basic_string = fixed_type<
    size, detail::fixed_basic_string
>;

// ---fixed_basic_int---
namespace detail
{
static constexpr char DIGITS[] =
"0001020304050607080910111213141516171819"
"2021222324252627282930313233343536373839"
"4041424344454647484950515253545556575859"
"6061626364656667686970717273747576777879"
"8081828384858687888990919293949596979899";

template<typename T> struct fixed_basic_unsigned
{
    using value_type = T;

    template<size_t size> fixed_basic_unsigned(
        std::array<char, size>& arr,
        T value
    )
    {
        auto it = arr.end();
        *--it = '\0';

        for (; value > 100; value /= 100)
        {
            const int index = static_cast<int>(value % 100 * 2);
            *--it = DIGITS[index + 1];
            *--it = DIGITS[index];
        }

        if (value < 10)
        {
            *--it = static_cast<char>('0' + value);
        }
        else
        {
            const int index = static_cast<int>(value * 2);
            *--it = DIGITS[index + 1];
            *--it = DIGITS[index];
        }

        const auto last = arr.begin();
        if (it != last)
            for (; --it != last;)
                *it = '\0';
    }

    template<size_t size> static value_type deserialize(
        const std::array<char, size>& arr
    )
    {
        value_type ret{};

        auto c = arr.data();
        for (; *c < '0' || *c > '9'; ++c);

        for (; *c != '\n'; ++c)
        {
            ret *= 10;
            ret += *c - '0';
        }
        return ret;
    }
};

template<typename T> struct fixed_basic_int
{
    using value_type = T;

    template<size_t b_size> fixed_basic_int(
        std::array<char, b_size>& arr,
        T value
    )
    {
        static_assert(std::is_integral<T>::value,
            "fixed_basic_int only support integral types!");

        arr.front() = '\0';
        if (value < 0)
        {
            value = 0 - value;
            arr.front() = '-';
        }

        fixed_basic_unsigned<T>{arr, value};
    }

    template<size_t b_size> static value_type deserialize(
        const std::array<char, b_size>& arr
    )
    {
        return fixed_basic_unsigned<value_type>::deserialize(arr);
    }
};
} // namespace detail

template<typename T> using fixed_basic_int = fixed_type<
    std::numeric_limits<T>::digits10 + 2,
    std::conditional_t<std::is_unsigned<T>::value,
        detail::fixed_basic_unsigned<T>,
        detail::fixed_basic_int<T>
    >
>;

// ---User Types---
using fixed_string = fixed_basic_string<sizeof(std::string)>;
using fixed_char = char;

// signed
using fixed_int = fixed_basic_int<int>;
using fixed_int8_t = fixed_basic_int<int8_t>;
using fixed_int16_t = fixed_basic_int<int16_t>;
using fixed_int32_t = fixed_basic_int<int32_t>;
using fixed_int64_t = fixed_basic_int<int64_t>;

// unsigned
using fixed_unsigned = fixed_basic_int<unsigned>;
using fixed_uint8_t = fixed_basic_int<uint8_t>;
using fixed_uint16_t = fixed_basic_int<uint16_t>;
using fixed_uint32_t = fixed_basic_int<uint32_t>;
using fixed_uint64_t = fixed_basic_int<uint64_t>;

} // namespace fixed

#endif // FMT_HPP_INCLUDED
