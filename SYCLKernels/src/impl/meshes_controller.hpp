#pragma once
#include <optional>
#include <unordered_map>
#include <guid.hpp>
#include <math_structs.hpp>
#include <non_owning_array.hpp>
#include "logger.hpp"
#include "sycl.hpp"

namespace native_voxelizer::sycl_kernels::impl {
  class meshes_controller final {
  public:
#pragma region Memory Management
    meshes_controller(sycl::queue& queue) : queue{queue} {}
    meshes_controller(meshes_controller const&) = delete;
    meshes_controller(meshes_controller&&) = default;
#pragma endregion

    ext::guid add_mesh(ext::non_owning_array<ext::vector3<float>> const& vertices);
    void update_mesh(ext::guid const& id, ext::non_owning_array<ext::vector3<float>> const& vertices);
    void remove_mesh(ext::guid const& id) noexcept;

    std::optional<sycl::buffer<sycl::vec<float, 3>>> pop_mesh(ext::guid const& id) noexcept;
    sycl::buffer<sycl::vec<float, 3>> const* get_mesh(ext::guid const& id) const noexcept;
    sycl::buffer<sycl::vec<float, 3>>* get_mesh(ext::guid const& id) noexcept;

  private:
    sycl::queue& queue;
    std::unordered_map<ext::guid, sycl::buffer<sycl::vec<float, 3>>> buffers_map;
  };
}
