#include "meshes_controller.hpp"
#include "vertices_converters.hpp"

using namespace std;
using namespace ext;

namespace native_voxelizer::sycl_kernels::impl {
  guid meshes_controller::add_mesh(non_owning_array<vector3<float>> const& vertices) {
    auto& [id, output_buffer]
        = *buffers_map.emplace(guid{}, sycl::range<1>{vertices.size()}).first;
    sycl::buffer<vector3<float>, 1> input_buffer = {sycl::range<1>{vertices.size()}};
    queue.submit(convert_to_sycl{input_buffer, output_buffer}).wait_and_throw();
    return id;
  }

  void meshes_controller::update_mesh(guid const& id, non_owning_array<vector3<float>> const& vertices) {
    sycl::buffer<sycl::vec<float, 3>>* output_buffer;
    if (auto it = buffers_map.find(id); it == buffers_map.end())
      output_buffer = &buffers_map.emplace(guid{}, sycl::range<1>{vertices.size()}).first->second;
    else
      output_buffer = &it->second;

    if (output_buffer->get_count() != vertices.size())
      *output_buffer = {sycl::range<1>{vertices.size()}};

    sycl::buffer<vector3<float>, 1> input_buffer = {sycl::range<1>{vertices.size()}};
    queue.submit(convert_to_sycl{input_buffer, *output_buffer}).wait_and_throw();
  }

  void meshes_controller::remove_mesh(guid const& id) noexcept {
    auto node = buffers_map.extract(id);
  }

  optional<sycl::buffer<sycl::vec<float, 3>>> meshes_controller::pop_mesh(guid const& id) noexcept {
    optional<sycl::buffer<sycl::vec<float, 3>>> value = nullopt;

    if (auto node = buffers_map.extract(id); node)
      value = node.mapped();

    return value;
  }

  sycl::buffer<sycl::vec<float, 3>> const* meshes_controller::get_mesh(guid const& id) const noexcept {
    sycl::buffer<sycl::vec<float, 3>> const* value = nullptr;

    if (auto it = buffers_map.find(id); it != buffers_map.end())
      value = &it->second;

    return value;
  }

  sycl::buffer<sycl::vec<float, 3>>* meshes_controller::get_mesh(guid const& id) noexcept {
    sycl::buffer<sycl::vec<float, 3>>* value = nullptr;

    if (auto it = buffers_map.find(id); it != buffers_map.end())
      value = &it->second;

    return value;
  }
}
