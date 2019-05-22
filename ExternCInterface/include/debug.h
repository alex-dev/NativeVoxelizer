#pragma once
#include < cstddef>
#include <cstdint>

namespace ext {
  template<typename T>
  struct vector3;
}

extern "C" {
bool get_debug_vertices(void* master, std::uint8_t* guid_buffer, std::size_t guid_length,
                        ext::vector3<float>* vertices_allocator(std::size_t)) noexcept;
}
