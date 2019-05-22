#include "meshes_controller.hpp"
#include <stdexcept>
#include "impl/meshes_controller.hpp"

using namespace std;
using namespace ext;

namespace native_voxelizer::sycl_kernels {
  meshes_controller::meshes_controller(class impl::meshes_controller& controller_impl) noexcept
      : controller_impl{controller_impl} {}

  optional<guid> meshes_controller::add_mesh(non_owning_array<vector3<float>> const& vertices) const
      noexcept {
    try {
      return controller_impl.add_mesh(vertices);
    } catch (exception&) {
      // TODO: log exception
      return nullopt;
    }
  }

  bool meshes_controller::update_mesh(guid const& id,
                                      non_owning_array<vector3<float>> const& vertices) const noexcept {
    try {
      controller_impl.update_mesh(id, vertices);
      return true;
    } catch (exception&) {
      // TODO: log exception
      return false;
    }
  }

  bool meshes_controller::remove_mesh(guid const& id) const noexcept {
    try {
      controller_impl.remove_mesh(id);
      return true;
    } catch (exception&) {
      // TODO: log exception
      return false;
    }
  }
}
