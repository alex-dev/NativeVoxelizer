#pragma once
#include <guid.hpp>
#include <math_structs.hpp>
#include <owning_array.hpp>
#include "impl.hpp"

namespace native_voxelizer::sycl_kernels {
  class debug_controller final {
  public:
    debug_controller(impl::debug_controller& controller_impl) noexcept;

    ext::owning_array<ext::vector3<float>> get_debug_vertices(ext::guid const& id) noexcept;

  private:
    impl::debug_controller& controller_impl;
  };
}
