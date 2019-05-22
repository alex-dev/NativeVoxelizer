#include "master.h"
#include <stdexcept>
#include <master_controller.hpp>

using namespace native_voxelizer::sycl_kernels;

extern "C" {
void* allocate() noexcept {
  try {
    return new master_controller();
  } catch (std::exception&) {
    // TODO: handle exception
    return nullptr;
  }
}

bool deallocate(void* master) noexcept {
  if (!master)
    return false;

  try {
    delete reinterpret_cast<master_controller*>(master);
    return true;
  } catch (std::exception&) {
    // TODO: handle exception
    return false;
  }
}
}
