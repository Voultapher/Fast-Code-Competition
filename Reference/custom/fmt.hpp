#ifndef FMT_HPP_INCLUDED
#define FMT_HPP_INCLUDED

#include <limits>
#include <cstdint>
#include <array>

/*class atoi_func
{
public:
    atoi_func(): value_() {}

    inline int value() const { return value_; }

    inline bool operator() (const char *str, size_t len)
    {
        value_ = 0;
        int sign = 1;
        if (str[0] == '-') { // handle negative
            sign = -1;
            ++str;
            --len;
        }

        switch (len) { // handle up to 10 digits, assume we're 32-bit
            case 10:    value_ += (str[len-10] - '0') * 1000000000;
            case  9:    value_ += (str[len- 9] - '0') * 100000000;
            case  8:    value_ += (str[len- 8] - '0') * 10000000;
            case  7:    value_ += (str[len- 7] - '0') * 1000000;
            case  6:    value_ += (str[len- 6] - '0') * 100000;
            case  5:    value_ += (str[len- 5] - '0') * 10000;
            case  4:    value_ += (str[len- 4] - '0') * 1000;
            case  3:    value_ += (str[len- 3] - '0') * 100;
            case  2:    value_ += (str[len- 2] - '0') * 10;
            case  1:    value_ += (str[len- 1] - '0');
                value_ *= sign;
                return value_ > 0;
            default:
                return false;
        }
    }
private:
    int value_;
};*/

namespace fixed
{

namespace detail
{
using fixed_buff_value_t = char;

static constexpr fixed_buff_value_t DIGITS[] =
"0001020304050607080910111213141516171819"
"2021222324252627282930313233343536373839"
"4041424344454647484950515253545556575859"
"6061626364656667686970717273747576777879"
"8081828384858687888990919293949596979899";

template<typename C> class fixed_basic_unsigned
{
public:
  using value_t = C;
  static constexpr auto value_size = std::numeric_limits<value_t>::digits10 + 1;

  using buff_value_t = fixed_buff_value_t;

  fixed_basic_unsigned(value_t value) noexcept : offset_(0)
	{
    auto it = buff_.end();
    *--it = '\0';

    for (; value >= 100; value /= 100)
    {
      const auto index = static_cast<int>(value % 100 * 2);
      *--it = DIGITS[index + 1];
      *--it = DIGITS[index];
    }

    if (value < 10)
    {
      *--it = static_cast<buff_value_t>('0' + value);
    }
    else
    {
      const auto index = static_cast<int>(value * 2);
      *--it = DIGITS[index + 1];
      *--it = DIGITS[index];
    }

    for (auto last = buff_.begin(); it != last; --it, ++offset_);
  }

  fixed_basic_unsigned(const fixed_basic_unsigned&) = default;
  fixed_basic_unsigned(fixed_basic_unsigned&&) = default;

  fixed_basic_unsigned& operator= (const fixed_basic_unsigned&) = default;
  fixed_basic_unsigned& operator= (fixed_basic_unsigned&&) = default;

  ~fixed_basic_unsigned() = default;

  // would be nicer with dot operator
  constexpr operator value_t() const noexcept
  {
    value_t ret{};

    auto c = buff_.data();
    for (; *c < '0' || *c > '9'; ++c);

    for (; *c != '\n'; ++c)
    {
      ret *= 10;
      ret += *c - '0';
    }
    return ret;
  }

  constexpr size_t size() noexcept { return value_size - offset_; }

  constexpr const buff_value_t* data() const noexcept
  {
    return buff_.data() + offset_;
  }

private:
  std::array<buff_value_t, value_size> buff_;
  int8_t offset_;
};

} // namespace detail

// unsigned
using fixed_unsigned = detail::fixed_basic_unsigned<unsigned>;
using fixed_uint8_t = detail::fixed_basic_unsigned<uint8_t>;
using fixed_uint16_t = detail::fixed_basic_unsigned<uint16_t>;
using fixed_uint32_t = detail::fixed_basic_unsigned<uint32_t>;
using fixed_uint64_t = detail::fixed_basic_unsigned<uint64_t>;

} // namespace fixed

#endif // FMT_HPP_INCLUDED
