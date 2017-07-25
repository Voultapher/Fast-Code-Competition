#ifndef FMT_HPP_INCLUDED
#define FMT_HPP_INCLUDED

#include <limits>
#include <cstdint>
#include <array>

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
  static constexpr auto value_size = std::numeric_limits<value_t>::digits10;

  using buff_value_t = fixed_buff_value_t;

  fixed_basic_unsigned(value_t value) noexcept : offset_(0)
	{

    auto it = buff_.end();

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
