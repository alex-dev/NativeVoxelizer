#include "master_controller.hpp"
#include "impl/master_controller.hpp"

using namespace std;

namespace native_voxelizer::sycl_kernels {
  master_controller::master_controller()
      : master{make_shared<impl::master_controller>("temp"), impl::logger::level::info},
        meshes{master->get_meshes_controller()}, debug{master->get_debug_controller()} {}
}
