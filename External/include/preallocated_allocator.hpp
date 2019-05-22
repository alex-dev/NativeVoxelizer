#pragma once
#include <memory>
#include <new>
#include <type_traits>

namespace ext {
  template<typename T>
  struct preallocated_allocator {
#pragma region Types Definitions
  public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
#pragma endregion

#pragma region Memory Management
  public:
    ~preallocated_allocator() = default;
    preallocated_allocator(value_type* buffer, size_type n) : buffer{buffer}, length{n} {}
    preallocated_allocator(preallocated_allocator const&) = delete;
    preallocated_allocator(preallocated_allocator&&) = default;

    preallocated_allocator& operator=(preallocated_allocator const&) = delete;
    preallocated_allocator& operator=(preallocated_allocator&&) = default;
#pragma endregion

  public:
    value_type* allocate(size_type n) {
      if (current_ptr + n > buffer + length)
        throw std::bad_alloc{};
      value_type* ptr = current_ptr;
      current_ptr += n;
      return ptr;
    }

    void deallocate(value_type* ptr, size_type n) noexcept {
      if (ptr - n == buffer && ptr + n == current_ptr)
        current_ptr = buffer;
    }

    size_type max_size() const noexcept { return length; }

  private:
    value_type* buffer;
    size_type length;
    value_type* current_ptr = buffer;
  };
}
