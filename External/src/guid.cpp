#include "guid.hpp"
#include <ctime>
#include <random>
#include <stdexcept>

using namespace std;

namespace {
  static minstd_rand random_engine{static_cast<uint32_t>(time(nullptr))};
  static uniform_int_distribution<uint32_t> random_distribution{};

  void to_char(uint8_t byte, char (&c)[2]) noexcept {
    constexpr char map[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    c[0] = map[byte & 0xF0];
    c[1] = map[byte & 0x0F];
  }

  void to_char(uint8_t byte, wchar_t (&c)[2]) noexcept {
    constexpr wchar_t map[16] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7',
                                 L'8', L'9', L'a', L'b', L'c', L'd', L'e', L'f'};

    c[0] = map[byte & 0xF0];
    c[1] = map[byte & 0x0F];
  }

#if __cpp_lib_char8_t >= 201811L
  void to_char(uint8_t byte, char8_t (&c)[2]) noexcept {
    constexpr char8_t map[16] = {u8'0', u8'1', u8'2', u8'3', u8'4', u8'5', u8'6', u8'7',
                                 u8'8', u8'9', u8'a', u8'b', u8'c', u8'd', u8'e', u8'f'};

    c[0] = map[byte & 0xF0];
    c[1] = map[byte & 0x0F];
  }
#endif

  void to_char(uint8_t byte, char16_t (&c)[2]) noexcept {
    constexpr char16_t map[16] = {u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7',
                                  u'8', u'9', u'a', u'b', u'c', u'd', u'e', u'f'};

    c[0] = map[byte & 0xF0];
    c[1] = map[byte & 0x0F];
  }

  void to_char(uint8_t byte, char32_t (&c)[2]) noexcept {
    constexpr char32_t map[16] = {U'0', U'1', U'2', U'3', U'4', U'5', U'6', U'7',
                                  U'8', U'9', U'a', U'b', U'c', U'd', U'e', U'f'};

    c[0] = map[byte & 0xF0];
    c[1] = map[byte & 0x0F];
  }

  uint8_t from_char(char const (&c)[2]) {
    array<uint8_t, 2> byte;

    for (size_t i = 0; i < 2; ++i)
      switch (c[i]) {
        case '0': byte[i] = 0; break;
        case '1': byte[i] = 1; break;
        case '2': byte[i] = 2; break;
        case '3': byte[i] = 3; break;
        case '4': byte[i] = 4; break;
        case '5': byte[i] = 5; break;
        case '6': byte[i] = 6; break;
        case '7': byte[i] = 7; break;
        case '8': byte[i] = 8; break;
        case '9': byte[i] = 9; break;
        case 'A': byte[i] = 10; break;
        case 'a': byte[i] = 10; break;
        case 'B': byte[i] = 11; break;
        case 'b': byte[i] = 11; break;
        case 'C': byte[i] = 12; break;
        case 'c': byte[i] = 12; break;
        case 'D': byte[i] = 13; break;
        case 'd': byte[i] = 13; break;
        case 'E': byte[i] = 14; break;
        case 'e': byte[i] = 14; break;
        case 'F': byte[i] = 15; break;
        case 'f': byte[i] = 15; break;
        default: throw invalid_argument{"Unknown hexadecimal digit."};
      }

    return (byte[0] << 4) + byte[1];
  }

  uint8_t from_char(wchar_t const (&c)[2]) {
    array<uint8_t, 2> byte;

    for (size_t i = 0; i < 2; ++i)
      switch (c[i]) {
        case L'0': byte[i] = 0; break;
        case L'1': byte[i] = 1; break;
        case L'2': byte[i] = 2; break;
        case L'3': byte[i] = 3; break;
        case L'4': byte[i] = 4; break;
        case L'5': byte[i] = 5; break;
        case L'6': byte[i] = 6; break;
        case L'7': byte[i] = 7; break;
        case L'8': byte[i] = 8; break;
        case L'9': byte[i] = 9; break;
        case L'A': byte[i] = 10; break;
        case L'a': byte[i] = 10; break;
        case L'B': byte[i] = 11; break;
        case L'b': byte[i] = 11; break;
        case L'C': byte[i] = 12; break;
        case L'c': byte[i] = 12; break;
        case L'D': byte[i] = 13; break;
        case L'd': byte[i] = 13; break;
        case L'E': byte[i] = 14; break;
        case L'e': byte[i] = 14; break;
        case L'F': byte[i] = 15; break;
        case L'f': byte[i] = 15; break;
        default: throw invalid_argument{"Unknown hexadecimal digit."};
      }

    return (byte[0] << 4) + byte[1];
  }

#if __cpp_lib_char8_t >= 201811L
  uint8_t from_char(char8_t (&c)[2]) {
    array<uint8_t, 2> byte;

    for (size_t i = 0; i < 2; ++i)
      switch (c[i]) {
        case u8'0': byte[i] = 0; break;
        case u8'1': byte[i] = 1; break;
        case u8'2': byte[i] = 2; break;
        case u8'3': byte[i] = 3; break;
        case u8'4': byte[i] = 4; break;
        case u8'5': byte[i] = 5; break;
        case u8'6': byte[i] = 6; break;
        case u8'7': byte[i] = 7; break;
        case u8'8': byte[i] = 8; break;
        case u8'9': byte[i] = 9; break;
        case u8'A': byte[i] = 10; break;
        case u8'a': byte[i] = 10; break;
        case u8'B': byte[i] = 11; break;
        case u8'b': byte[i] = 11; break;
        case u8'C': byte[i] = 12; break;
        case u8'c': byte[i] = 12; break;
        case u8'D': byte[i] = 13; break;
        case u8'd': byte[i] = 13; break;
        case u8'E': byte[i] = 14; break;
        case u8'e': byte[i] = 14; break;
        case u8'F': byte[i] = 15; break;
        case u8'f': byte[i] = 15; break;
        default: throw invalid_argument{"Unknown hexadecimal digit."};
      }

    return (byte[0] << 4) + byte[1];
  }
#endif

  uint8_t from_char(char16_t const (&c)[2]) {
    array<uint8_t, 2> byte;

    for (size_t i = 0; i < 2; ++i)
      switch (c[i]) {
        case u'0': byte[i] = 0; break;
        case u'1': byte[i] = 1; break;
        case u'2': byte[i] = 2; break;
        case u'3': byte[i] = 3; break;
        case u'4': byte[i] = 4; break;
        case u'5': byte[i] = 5; break;
        case u'6': byte[i] = 6; break;
        case u'7': byte[i] = 7; break;
        case u'8': byte[i] = 8; break;
        case u'9': byte[i] = 9; break;
        case u'A': byte[i] = 10; break;
        case u'a': byte[i] = 10; break;
        case u'B': byte[i] = 11; break;
        case u'b': byte[i] = 11; break;
        case u'C': byte[i] = 12; break;
        case u'c': byte[i] = 12; break;
        case u'D': byte[i] = 13; break;
        case u'd': byte[i] = 13; break;
        case u'E': byte[i] = 14; break;
        case u'e': byte[i] = 14; break;
        case u'F': byte[i] = 15; break;
        case u'f': byte[i] = 15; break;
        default: throw invalid_argument{"Unknown hexadecimal digit."};
      }

    return (byte[0] << 4) + byte[1];
  }

  uint8_t from_char(char32_t const (&c)[2]) {
    array<uint8_t, 2> byte;

    for (size_t i = 0; i < 2; ++i)
      switch (c[i]) {
        case U'0': byte[i] = 0; break;
        case U'1': byte[i] = 1; break;
        case U'2': byte[i] = 2; break;
        case U'3': byte[i] = 3; break;
        case U'4': byte[i] = 4; break;
        case U'5': byte[i] = 5; break;
        case U'6': byte[i] = 6; break;
        case U'7': byte[i] = 7; break;
        case U'8': byte[i] = 8; break;
        case U'9': byte[i] = 9; break;
        case U'A': byte[i] = 10; break;
        case U'a': byte[i] = 10; break;
        case U'B': byte[i] = 11; break;
        case U'b': byte[i] = 11; break;
        case U'C': byte[i] = 12; break;
        case U'c': byte[i] = 12; break;
        case U'D': byte[i] = 13; break;
        case U'd': byte[i] = 13; break;
        case U'E': byte[i] = 14; break;
        case U'e': byte[i] = 14; break;
        case U'F': byte[i] = 15; break;
        case U'f': byte[i] = 15; break;
        default: throw invalid_argument{"Unknown hexadecimal digit."};
      }

    return (byte[0] << 4) + byte[1];
  }

  template<typename CharType>
  basic_string<CharType> to_string(array<uint8_t, 16> const& bytes, CharType separator) noexcept {
    basic_string<CharType> str{36, separator};
    size_t s = 0;

    for (size_t i = 0; i < 4; ++i, s += 2)
      to_char(bytes[i], reinterpret_cast<CharType(&)[2]>(str[s]));
    ++s;

    for (size_t j = 1; j < 4; ++j) {
      for (size_t i = 4 + 2 * (j - 1); i < 4 + 2 * j; ++i, s += 2)
        to_char(bytes[i], reinterpret_cast<CharType(&)[2]>(str[s]));
      ++s;
    }

    for (size_t i = 4 + 2 * 3; i < 16; ++i, s += 2)
      to_char(bytes[i], reinterpret_cast<CharType(&)[2]>(str[s]));

    return str;
  }

  template<typename CharType>
  array<uint8_t, 16> from_string(basic_string_view<CharType> const& str,
                                 CharType separator, basic_string_view<CharType> wrappers) {
    array<uint8_t, 16> bytes;
    bool has_separator = false;
    size_t start = str.find_first_not_of(wrappers);
    size_t s = start;

    for (size_t i = 0; i < 4; ++i, s += 2)
      bytes[i] = from_char(reinterpret_cast<CharType const(&)[2]>(str[s]));
    if (str[s] == separator) {
      has_separator = true;
      ++s;
    }

    for (size_t j = 1; j < 4; ++j) {
      for (size_t i = 4 + 2 * (j - 1); i < 4 + 2 * j; ++i, s += 2)
        bytes[i] = from_char(reinterpret_cast<CharType const(&)[2]>(str[s]));
      if (has_separator) {
        if (str[s] != separator)
          throw invalid_argument{"Missing GUID separator '-'."};
        ++s;
      }
    }

    for (size_t i = 4 + 2 * 3; i < 16; ++i, s += 2)
      bytes[i] = from_char(reinterpret_cast<CharType const(&)[2]>(str[s]));

    if (str.length() - start != str.find_last_not_of(wrappers))
      // Actual order balancing is not validated because it would take too much time or
      // use regexes, which is too slow for this kind of parsing.
      throw invalid_argument{"Unbalanced wrappers."};

    return bytes;
  }
}

namespace ext {
  string guid::convert_to_string(array<uint8_t, 16> const& bytes) noexcept {
    return ::to_string(bytes, '-');
  }

  array<uint8_t, 16> guid::convert_from_string(string_view const& str) {
    return ::from_string(str, '-', "{()}"sv);
  }

  array<uint8_t, 16> guid::generate_v4_variant1() noexcept {
    array<uint8_t, 16> bytes;

    for (uint32_t& i : reinterpret_cast<uint32_t(&)[4]>(*bytes.data()))
      i = random_distribution(random_engine);

    {
      uint16_t(&ref)[8] = reinterpret_cast<uint16_t(&)[8]>(*bytes.data());
      ref[3] = (ref[3] & 0x0FFF) | 0x4000;
      ref[4] = (ref[4] & 0x3FFF) | 0x8000;
    }

    return bytes;
  }
}
