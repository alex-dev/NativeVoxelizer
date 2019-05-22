#pragma once
#include <lambda_allocated_allocator.hpp>
#include <owning_array.hpp>
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
    debug_controller(logger& logger, sycl::queue& queue, meshes_controller& meshes_controller) noexcept
        : logger{logger}, queue{queue}, meshes_controller{meshes_controller} {}
    debug_controller(debug_controller const&) = delete;
    debug_controller(debug_controller&&) = default;
#pragma endregion

#pragma region Accessors
  public:
    logger& get_logger() noexcept { return logger; }
    logger const& get_logger() const noexcept { return logger; }
#pragma endregion

  public:
    vector_collection get_debug_vertices(ext::guid const& id, allocator&& allocator) const;

  private:
    logger& logger;
    sycl::queue& queue;
    meshes_controller& meshes_controller;
  };
}
