#include "debug_controller.hpp"
#include <owning_array.hpp>
#include "impl/debug_controller.hpp"

using namespace std;
using namespace ext;

namespace native_voxelizer::sycl_kernels {
  debug_controller::debug_controller(impl::debug_controller& controller_impl) noexcept
      : controller_impl{controller_impl} {}

  owning_array<vector3<float>> debug_controller::get_debug_vertices(guid const& id) noexcept {
    try {
      return controller_impl.get_debug_vertices(id);
    } catch (exception&) {
      // TODO: log exception
      return {};
    }
  }
}
