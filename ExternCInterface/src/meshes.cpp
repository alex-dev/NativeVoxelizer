#include "meshes.h"
#include <stdexcept>
#include <master_controller.hpp>
#include "impl/marshal_guid.hpp"

using namespace std;
using namespace ext;
using namespace native_voxelizer;
using namespace native_voxelizer::sycl_kernels;

namespace native_voxelizer {
  meshes_controller& get_controller(void* master) {
    return reinterpret_cast<master_controller*>(master)->get_meshes_controller();
  }
}

extern "C" {
bool add_mesh(void* master, vector3<float>* vertices_buffer, size_t vertices_length,
              uint8_t* guid_buffer, size_t guid_length) noexcept {
  if (!master || !vertices_buffer || !vertices_length || !guid_buffer || guid_length != 16)
    return false;
  auto guid = get_controller(master).add_mesh({vertices_buffer, vertices_length});

  if (!guid)
    return false;

  auto bytes = guid_write(*guid, guid_buffer, guid_length);

  return static_cast<bool>(bytes);
}

bool update_mesh(void* master, uint8_t* guid_buffer, size_t guid_length,
                 vector3<float>* vertices_buffer, size_t vertices_length) noexcept {
  if (!master || !vertices_buffer || !vertices_length || !guid_buffer || guid_length != 16)
    return false;

  auto guid = guid_read(guid_buffer, guid_length);

  if (!guid)
    return false;

  return get_controller(master).update_mesh(*guid, {vertices_buffer, vertices_length});
}

bool remove_mesh(void* master, uint8_t* guid_buffer, size_t guid_length) noexcept {
  if (!master || !guid_buffer || guid_length != 16)
    return false;

  auto guid = guid_read(guid_buffer, guid_length);

  if (!guid)
    return false;

  return get_controller(master).remove_mesh(*guid);
}
}
