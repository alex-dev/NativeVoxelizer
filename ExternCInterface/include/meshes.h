#pragma once
#include <cstddef>
#include <cstdint>

namespace ext {
  template<typename T>
  struct vector3;
}

extern "C" {
bool add_mesh(void* master, ext::vector3<float>* vertices_buffer, std::size_t vertices_length,
              std::uint8_t* guid_buffer, std::size_t guid_length) noexcept;
bool update_mesh(void* master, std::uint8_t* guid_buffer, std::size_t guid_length,
                 ext::vector3<float>* vertices_buffer, std::size_t vertices_length) noexcept;
bool remove_mesh(void* master, std::uint8_t* guid_buffer, std::size_t guid_length) noexcept;
}
