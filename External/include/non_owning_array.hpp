#pragma once
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>

namespace ext {
  template<typename T>
  class non_owning_array {
#pragma region Types Definitions
  public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

  private:
    static constexpr bool is_nothrow_copy = std::is_nothrow_copy_constructible_v<value_type>;
#pragma endregion

#pragma region Iterators Definitions
  public:
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
#pragma endregion

#pragma region Memory Management
  public:
    template<size_type N>
    non_owning_array(value_type (&array)[N]) noexcept : non_owning_array{array, N} {}

    non_owning_array(value_type* array, size_type length) noexcept
        : length{length}, ptr{length ? array : nullptr} {}
#pragma endregion

#pragma region Element Access
  public:
    reference at(size_type position) {
      if (position >= length)
        throw std::out_of_range{"Position was out of bound."};

      return *(ptr + position);
    }

    const_reference at(size_type position) const {
      if (position >= length)
        throw std::out_of_range{"Position was out of bound."};

      return *(ptr + position);
    }

    reference operator[](size_type position) noexcept { return *(ptr + position); }
    const_reference operator[](size_type position) const noexcept {
      return *(ptr + position);
    }

    reference front() noexcept { return *ptr; }
    const_reference front() const noexcept { return *ptr; }
    reference back() noexcept { return *(ptr + length - 1); }
    const_reference back() const noexcept { return *(ptr + length - 1); }

    pointer data() noexcept { return ptr; }
    const_pointer data() const noexcept { return ptr; }
#pragma endregion

#pragma region Capacity
  public:
    bool empty() const noexcept { return length == 0; }
    size_type size() const noexcept { return length; }
    size_type max_size() const noexcept { return length; }
#pragma endregion

#pragma region Iterators
  private:
    reverse_iterator reverse(iterator it) noexcept { return reverse_iterator{it}; }
    const_reverse_iterator reverse(const_iterator it) const noexcept {
      return const_reverse_iterator{it};
    }

  public:
    iterator begin() noexcept { return ptr; }
    const_iterator begin() const noexcept { return cbegin(); }
    const_iterator cbegin() const noexcept { return ptr; }
    iterator end() noexcept { return ptr + length; }
    const_iterator end() const noexcept { return cend(); }
    const_iterator cend() const noexcept { return ptr + length; }

    reverse_iterator rbegin() noexcept { return reverse(begin()); }
    const_reverse_iterator rbegin() const noexcept { return reverse(begin()); }
    const_reverse_iterator crbegin() const noexcept { return reverse(begin()); }
    reverse_iterator rend() noexcept { return reverse(begin()); }
    const_reverse_iterator rend() const noexcept { return reverse(begin()); }
    const_reverse_iterator crend() const noexcept { return reverse(cbegin()); }
#pragma endregion

#pragma region Operations
  public:
    void fill(value_type const& value) noexcept(is_nothrow_copy) {
      std::fill(begin(), end(), value);
    }
#pragma endregion

  private:
    size_type length;
    value_type* ptr;
  };
}
