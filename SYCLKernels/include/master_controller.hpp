#pragma once
#include <memory>
#include "debug_controller.hpp"
#include "impl.hpp"
#include "meshes_controller.hpp"

namespace native_voxelizer::sycl_kernels {
  class master_controller final {
  public:
    master_controller();

    debug_controller& get_debug_controller() noexcept { return debug; }
    meshes_controller& get_meshes_controller() noexcept { return meshes; }

  private:
    std::shared_ptr<impl::master_controller> master;
    meshes_controller meshes;
    debug_controller debug;
  };
}
