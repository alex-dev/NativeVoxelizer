#pragma once
#include <lambda_allocated_allocator.hpp>
#include <owning_array.hpp>
#include "logger.hpp"
#include "meshes_controller.hpp"

namespace native_voxelizer::sycl_kernels::impl {
  class debug_controller final {
#pragma region Type Definitions
  public:
    using allocator = ext::lambda_allocated_allocator<ext::vector3<float>>;
    using vector_collection = ext::owning_array<ext::vector3<float>, allocator>;
#pragma endregion

#pragma region Memory Management
  public:
    debug_controller(ext::guid const& guid, sycl::queue& queue, meshes_controller& meshes_controller) noexcept
        : logger{guid, "controller.debug"}, queue{&queue}, meshes_controller{&meshes_controller} {}
    debug_controller(debug_controller const&) = delete;
    debug_controller(debug_controller&&) = default;

	debug_controller& operator=(debug_controller const&) = delete;
	debug_controller& operator=(debug_controller&&) = default;
#pragma endregion

  public:
    vector_collection get_debug_vertices(ext::guid const& id, allocator&& allocator) const;

  private:
    logger logger;
    sycl::queue* queue;
    meshes_controller* meshes_controller;
  };
}
