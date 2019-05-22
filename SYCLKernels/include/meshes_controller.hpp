#pragma once
#include <optional>
#include <guid.hpp>
#include <math_structs.hpp>
#include <non_owning_array.hpp>
#include "impl.hpp"

namespace native_voxelizer::sycl_kernels {
  class meshes_controller final {
  public:
    meshes_controller(impl::meshes_controller& controller_impl) noexcept;

    std::optional<ext::guid>
        add_mesh(ext::non_owning_array<ext::vector3<float>> const& vertices) const noexcept;
    bool update_mesh(ext::guid const& id,
                     ext::non_owning_array<ext::vector3<float>> const& vertices) const noexcept;
    bool remove_mesh(ext::guid const& id) const noexcept;

  private:
    impl::meshes_controller& controller_impl;
  };
}
