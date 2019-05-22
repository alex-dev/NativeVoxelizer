#include "vertices_converters.hpp"

using namespace ext;

namespace {
  using namespace native_voxelizer::sycl_kernels;

  template<typename T>
  sycl::vec<T, 3> _convert_to_sycl(vector3<T> const& vector) noexcept {
    return {vector.x, vector.y, vector.z};
  }

  template<typename T>
  vector3<T> _convert_from_sycl(sycl::vec<T, 3> const& vector) noexcept {
    return {vector.x(), vector.y(), vector.z()};
  }
}

namespace native_voxelizer::sycl_kernels::impl {
  using mode = sycl::access::mode;

  convert_to_sycl::convert_to_sycl(sycl::buffer<vector3<float>, 1>& input_buffer,
                                   sycl::buffer<sycl::vec<float, 3>, 1>& output_buffer)
      : input_buffer{input_buffer}, output_buffer{output_buffer} {}

  void convert_to_sycl::operator()(sycl::handler& command_group) {
    command_group.parallel_for<class push_host_vector_to_global_memory>(
        sycl::range<1>(input_buffer.get_count()),
        [input_accessor = input_buffer.get_access<mode::read>(command_group),
         output_accessor = output_buffer.get_access<mode::discard_write>(command_group)](
            sycl::item<1> item) mutable {
          size_t const id = item.get_linear_id();
          output_accessor[id] = _convert_to_sycl(input_accessor[id]);
        });
  }

  convert_from_sycl::convert_from_sycl(sycl::buffer<sycl::vec<float, 3>, 1>& input_buffer,
                                       sycl::buffer<vector3<float>, 1> output_buffer)
      : input_buffer{input_buffer}, output_buffer{output_buffer} {}

  void convert_from_sycl::operator()(sycl::handler& command_group) {
    command_group.parallel_for<class pull_host_vector_from_global_memory>(
        sycl::range<1>(input_buffer.get_count()),
        [input_accessor = input_buffer.get_access<mode::read>(command_group),
         output_accessor = output_buffer.get_access<mode::discard_write>(command_group)](
            sycl::item<1> item) mutable {
          size_t const id = item.get_linear_id();
          output_accessor[id] = _convert_from_sycl(input_accessor[id]);
        });
  }
}
