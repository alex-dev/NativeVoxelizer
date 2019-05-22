#pragma once
#include <cstddef>
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace ext {
  template<typename T, typename Allocator = std::allocator<T>>
  class owning_array {
    using allocator_traits = std::allocator_traits<Allocator>;

    template<typename Iterator>
    static constexpr bool is_random_access_iterator
        = std::is_base_of_v<std::random_access_iterator_tag, std::iterator_traits<Iterator>::iterator_category>;
    static constexpr bool propagate_allocator_on_copy
        = allocator_traits::propagate_on_container_copy_assignment::value;
    static constexpr bool propagate_allocator_on_move
        = allocator_traits::propagate_on_container_move_assignment::value;
    static constexpr bool propagate_allocator_on_swap
        = allocator_traits::propagate_on_container_swap::value;
    static constexpr bool allocator_is_always_equal = allocator_traits::is_always_equal::value;

#pragma region Types Definitions
  public:
    using allocator_type = Allocator;
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
  private:
    void cleanup() {
      if (ptr) {
        std::destroy_n(ptr, length);
        alloc.deallocate(ptr, length);
      }
    }

  public:
    ~owning_array() { cleanup(); }

    owning_array() noexcept(std::is_nothrow_default_constructible_v<Allocator>)
        : owning_array{Allocator{}} {}
    explicit owning_array(Allocator alloc) : alloc{std::move(alloc)} {}

    owning_array(size_type length, value_type const& value, Allocator alloc = {})
        : alloc{std::move(alloc)}, length{length} {
      std::uninitialized_fill_n(ptr, length, value);
    }

    explicit owning_array(size_type length, Allocator alloc = {})
        : alloc{std::move(alloc)}, length{length} {
      std::uninitialized_default_construct_n(ptr, length);
    }

    template<typename InputIt, typename = std::enable_if_t<is_random_access_iterator<InputIt>>>
    owning_array(InputIt first, InputIt last, Allocator alloc = {})
        : alloc{std::move(alloc)}, length{static_cast<size_type>(last - first)} {
      std::uninitialized_copy(first, last, ptr);
    }

    owning_array(owning_array const& other)
        : owning_array{other,
                       allocator_traits::select_on_container_copy_construction(other.alloc)} {}
    owning_array(owning_array const& other, Allocator alloc)
        : alloc{std::move(alloc)}, length{other.length} {
      std::uninitialized_copy_n(other.ptr, length, ptr);
    }

    owning_array(owning_array&& other) noexcept
        : alloc{std::move(other.alloc)}, length{other.length}, ptr{other.ptr} {
      other.length = 0;
      other.ptr = nullptr;
    }
    owning_array(owning_array&& other, Allocator alloc)
        : alloc{std::move(alloc)}, length{other.length}, ptr{nullptr} {
      if (allocator_is_always_equal || this->alloc == other.alloc) {
        ptr = other.ptr;
        other.length = 0;
        other.ptr = nullptr;
      } else {
        ptr = this->alloc.allocate(length);
        std::uninitialized_move_n(other.ptr, length, ptr);
      }
    }

    owning_array(std::initializer_list<T> init, Allocator alloc = {})
        : alloc{std::move(alloc)}, length{init.size()} {
      std::uninitialized_copy(init.begin(), init.end(), ptr);
    }

    owning_array& operator=(owning_array const& rhs) {
      cleanup();
      length = rhs.length;

      if constexpr (propagate_allocator_on_copy && !allocator_is_always_equal) {
        alloc = rhs.alloc;
      }

      ptr = alloc.allocate(length);
      std::uninitialized_copy_n(rhs.ptr, length, ptr);

      return *this;
    }

    owning_array& operator=(owning_array&& rhs) noexcept(propagate_allocator_on_move
                                                         || allocator_is_always_equal) {
      cleanup();

      length = rhs.length;

      if constexpr (propagate_allocator_on_move)
        alloc = std::move(rhs.alloc);

      if (allocator_is_always_equal || propagate_allocator_on_move || alloc == rhs.alloc) {
        ptr = rhs.ptr;
        rhs.length = 0;
        rhs.ptr = nullptr;
      } else {
        ptr = alloc.allocate(length);
        std::uninitialized_move_n(rhs.ptr, length, ptr);
      }

      return *this;
    }

    owning_array& operator=(std::initializer_list<T> rhs) {
      cleanup();

      length = rhs.size();
      ptr = alloc.allocate(length);
      std::uninitialized_copy(rhs.begin(), rhs.end(), ptr);

      return *this;
    }

    friend void swap(owning_array& lhs,
                     owning_array& rhs) noexcept(owning_array::propagate_allocator_on_swap
                                                 || owning_array::allocator_is_always_equal) {
      if constexpr (owning_array::propagate_allocator_on_swap)
        swap(lhs.alloc, rhs.alloc);

      std::swap(lhs.ptr, rhs.ptr);
      std::swap(lhs.length, rhs.length);
    }

    pointer move_data(Allocator& allocator) {
      pointer value = ptr;

      if (!allocator_is_always_equal && alloc != allocator) {
        swap(alloc, allocator);
      }

      ptr = nullptr;
      length = 0;

      return value;
    }
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
    Allocator alloc;
    size_type length = 0;
    value_type* ptr = alloc.allocate(length);
  };
}
