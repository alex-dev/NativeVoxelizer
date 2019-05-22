#include "debug.h"
#include <stdexcept>
#include <master_controller.hpp>
#include "impl/marshal_guid.hpp"

using namespace std;
using namespace ext;
using namespace native_voxelizer;
using namespace native_voxelizer::sycl_kernels;

namespace native_voxelizer {
  debug_controller& get_controller(void* master) {
    return reinterpret_cast<master_controller*>(master)->get_debug_controller();
  }
}

extern "C" {
bool get_debug_vertices(void* master, uint8_t* guid_buffer, size_t guid_length,
                        vector3<float>* vertices_allocator(size_t)) noexcept {
  if (!master || !vertices_allocator || !guid_buffer || guid_length != 16)
    return false;

  auto guid = guid_read(guid_buffer, guid_length);

  if (!guid)
    return false;

  auto vertices = get_controller(master).get_debug_vertices(*guid);

  if (!vertices.size())
    return false;

  auto* vertices_buffer = vertices_allocator(vertices.size());

  if (!vertices_buffer)
    return false;

  try {
    owning_array<vector3<float>, preallocated_allocator<vector3<float>>>{
        vertices.begin(), vertices.end(), {vertices_buffer, vertices.size()}};
  } catch (std::exception&) {
    return false;
  }

  return true;
}
}
