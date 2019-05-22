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

	master_controller& operator=(master_controller const&) = delete;
	master_controller& operator=(master_controller&&) = default;
#pragma endregion

#pragma region Accessors
  public:
    debug_controller& get_debug_controller() noexcept { return debug; }
    debug_controller const& get_debug_controller() const noexcept { return debug; }
    meshes_controller& get_meshes_controller() noexcept { return meshes; }
    meshes_controller const& get_meshes_controller() const noexcept { return meshes; }
#pragma endregion

  private:
	ext::guid guid = {};
    sycl::queue queue = {sycl::default_selector{},
                         [logger = logger{guid, "sycl.exception_handler"}](sycl::exception_list exceptions) mutable {
                           for (std::exception_ptr const& exception_ptr : exceptions)
                             try {
                               std::rethrow_exception(exception_ptr);
                             } catch (sycl::exception const& exception) {
                               logger.error("handle_exception") << exception.what();
                             }
                         }};
    meshes_controller meshes = {guid, queue};
    debug_controller debug = {guid, queue, meshes};
  };
}
