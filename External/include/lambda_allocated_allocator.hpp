#pragma once
#include <functional>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>

namespace ext {
  template<typename T>
  struct lambda_allocated_allocator {
#pragma region Types Definitions
  public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    using allocator_type = std::function<T*(size_type)>;
    using deallocator_type = std::function<void(T*, size_type)>;
#pragma endregion

#pragma region Memory Management
  public:
    ~lambda_allocated_allocator() = default;
    lambda_allocated_allocator(allocator_type allocator, deallocator_type deallocator)
        : allocator{allocator}, deallocator{deallocator} {}
    lambda_allocated_allocator(lambda_allocated_allocator const&) = delete;
    lambda_allocated_allocator(lambda_allocated_allocator&&) = default;

    lambda_allocated_allocator& operator=(lambda_allocated_allocator const&) = delete;
    lambda_allocated_allocator& operator=(lambda_allocated_allocator&&) = default;
#pragma endregion

  public:
    value_type* allocate(size_type n) {
      if (buffer || !(buffer = allocator(n)))
        throw std::bad_alloc{};

      length = n;
      return buffer;
    }

    void deallocate(value_type* ptr, size_type n) noexcept {
      if (buffer && ptr == buffer)
        deallocator(buffer, length);
    }

    size_type max_size() const noexcept { return length; }

  private:
    allocator_type allocator;
    deallocator_type deallocator;
    value_type* buffer = nullptr;
    size_type length = std::numeric_limits<size_type>::max();
  };
}
