#pragma once
#include <optional>
#include "debug_controller.hpp"
#include "logger.hpp"
#include "meshes_controller.hpp"

namespace native_voxelizer::sycl_kernels::impl {
  class master_controller final {
#pragma region Memory Management
  public:
    master_controller() = default;
    master_controller(master_controller const&) = delete;
    master_controller(master_controller&&) = default;
#pragma endregion

#pragma region Accessors
  public:
    debug_controller& get_debug_controller() noexcept { return debug; }
    debug_controller const& get_debug_controller() const noexcept { return debug; }
    meshes_controller& get_meshes_controller() noexcept { return meshes; }
    meshes_controller const& get_meshes_controller() const noexcept { return meshes; }
#pragma endregion

  private:
    sycl::queue queue = {sycl::default_selector{},
                         [logger = logger{{}, "sycl.exception_handler"}](sycl::exception_list exceptions) mutable {
                           for (std::exception_ptr const& exception_ptr : exceptions)
                             try {
                               std::rethrow_exception(exception_ptr);
                             } catch (sycl::exception const& exception) {
                               logger.error("exception") << exception.what();
                             }
                         }};
    meshes_controller meshes = {queue};
    debug_controller debug = {queue, meshes};
  };
}
