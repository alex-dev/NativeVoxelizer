#include <algorithm>
#include <array>
#include <optional>
#include <guid.hpp>
#include <owning_array.hpp>
#include <preallocated_allocator.hpp>

namespace native_voxelizer {
  inline std::optional<ext::owning_array<std::uint8_t, ext::preallocated_allocator<std::uint8_t>>>
      guid_write(ext::guid const& guid, std::uint8_t* buffer, std::size_t buffer_length) noexcept {
    if (!buffer || buffer_length != 16)
      return std::nullopt;

    std::array<std::uint8_t, 16> guid_data = guid;
    return {{guid_data.begin(), guid_data.end(), {buffer, buffer_length}}};
  }

  inline std::optional<ext::guid> guid_read(std::uint8_t* buffer, std::size_t buffer_length) noexcept {
    if (!buffer || buffer_length != 16)
      return std::nullopt;

    std::array<std::uint8_t, 16> guid_data;
    std::copy(buffer, buffer + buffer_length, guid_data.begin());
    return {ext::guid{std::move(guid_data)}};
  }
}
