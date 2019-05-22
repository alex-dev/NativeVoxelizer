#pragma once
#include <type_traits>

namespace ext {
  template<typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
  struct bitflag {
  private:
    using type = std::underlying_type_t<Enum>;

#pragma region Enum Conversions
  public:
    constexpr bitflag(Enum value) noexcept : value{static_cast<type>(value)} {}
    constexpr operator Enum() const noexcept { return static_cast<Enum>(value); }
#pragma endregion

#pragma region Boolean Operators
  public:
    constexpr operator bool() const noexcept { return static_cast<bool>(value); }
    constexpr bool operator!() const noexcept { return !operator bool(); }
#pragma endregion

#pragma region Binairy Operators
  public:
    constexpr bitflag operator~() const noexcept { return ~value; }
    friend constexpr bitflag operator|(bitflag lhs, bitflag rhs) noexcept {
      return lhs.value | rhs.value;
    }
    friend constexpr bitflag operator&(bitflag lhs, bitflag rhs) noexcept {
      return lhs.value & rhs.value;
    }
    friend constexpr bitflag operator^(bitflag value, bitflag rhs) noexcept {
      return lhs.value ^ rhs.value;
    }
#pragma endregion

#pragma region Compound Assignments
  public:
    constexpr bitflag& operator|=(bitflag other) noexcept {
      value |= other.value;
      return *this;
    }
    constexpr bitflag& operator&=(bitflag other) noexcept {
      value &= other.value;
      return *this;
    }
    constexpr bitflag& operator^=(bitflag other) noexcept {
      value ^= other.value;
      return *this;
    }
#pragma endregion

  private:
    constexpr bitflag(type value) noexcept : value{value} {}

    type value = 0;
  };
}
