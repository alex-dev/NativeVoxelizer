#pragma once
#include <math_structs.hpp>
#include "sycl.hpp"

namespace native_voxelizer::sycl_kernels::impl {
  struct convert_to_sycl {
    convert_to_sycl(sycl::buffer<ext::vector3<float>, 1>& input_buffer,
                    sycl::buffer<sycl::vec<float, 3>, 1>& output_buffer);
    void operator()(sycl::handler& command_group);

  private:
    sycl::buffer<ext::vector3<float>, 1>& input_buffer;
    sycl::buffer<sycl::vec<float, 3>, 1>& output_buffer;
  };

  struct convert_from_sycl {
    convert_from_sycl(sycl::buffer<sycl::vec<float, 3>, 1>& input_buffer,
                      sycl::buffer<ext::vector3<float>, 1> output_buffer);
    void operator()(sycl::handler& command_group);

  private:
    sycl::buffer<sycl::vec<float, 3>, 1>& input_buffer;
    sycl::buffer<ext::vector3<float>, 1>& output_buffer;
  };
}
