#pragma once
#include <cstdint>
#include <array>
#include <string>
#include <string_view>

namespace ext {
  struct guid {
  private:
    friend std::hash<guid>;

    static std::array<std::uint8_t, 16> generate_v4_variant1() noexcept;
    static std::string convert_to_string(std::array<std::uint8_t, 16> const& bytes) noexcept;
    static std::array<std::uint8_t, 16> convert_from_string(std::string_view const& str);

  public:
    guid() noexcept : guid{generate_v4_variant1()} {}

#pragma region Bytes Array Conversions
  public:
    constexpr explicit guid(std::array<std::uint8_t, 16> const& bytes) noexcept
        : bytes{bytes} {}
    constexpr explicit guid(std::array<std::uint8_t, 16>&& bytes) noexcept
        : bytes{std::move(bytes)} {}

    constexpr operator std::array<std::uint8_t, 16>() const noexcept { return bytes; }
#pragma endregion

#pragma region String Conversions
  public:
    explicit guid(std::string_view const& str) : guid{convert_from_string(str)} {}
    operator std::string() const noexcept { return convert_to_string(bytes); }
#pragma endregion

#pragma region Comparaison Operators
    friend bool operator==(guid const& lhs, guid const& rhs) {
      return lhs.bytes == rhs.bytes;
    }

    friend bool operator!=(guid const& lhs, guid const& rhs) {
      return !operator==(lhs, rhs);
    }
#pragma endregion

  private:
    std::array<std::uint8_t, 16> bytes;
  };
}

namespace std {
  template<>
  struct hash<ext::guid> {
    size_t operator()(ext::guid const& guid) const noexcept {
      uint64_t const(&ptr)[2] = reinterpret_cast<uint64_t const(&)[2]>(*guid.bytes.data());
      return hash_array(ptr[0], ptr[1]);
    }

  private:
    size_t hash_array(uint64_t const& val) const noexcept { return uint64_hash(val); }
    template<typename... Rest>
    size_t hash_array(uint64_t const& val, Rest const&... rest) const noexcept {
      size_t seed = hash_array(rest...);
      seed ^= hash_array(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      return seed;
    }

#if __has_cpp_attribute(no_unique_address)
    [[no_unique_address]] hash<uint64_t> uint64_hash;
#else
    hash<uint64_t> uint64_hash;
#endif
  };
}
