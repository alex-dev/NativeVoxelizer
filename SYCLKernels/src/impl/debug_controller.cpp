#include "debug_controller.hpp"
#include "sycl.hpp"

using namespace ext;

namespace {
  using namespace native_voxelizer::sycl_kernels::sycl;
  
  vector3<float> convert_from_sycl(vec<float, 3> const& vector) {
    return {vector.x(), vector.y(), vector.z()};
  }
}

namespace native_voxelizer::sycl_kernels::impl {
  using mode = sycl::access::mode;
  using vector_collection_type = debug_controller::vector_collection;
  using allocator_type = debug_controller::allocator;

  vector_collection debug_controller::get_debug_vertices(guid const& id, allocator&& allocator) const {
    auto* const input_buffer = meshes_controller.get_mesh(id);
    size_t length = input_buffer ? input_buffer->get_count() : 0;
	vector_collection_type output{length, std::forward<allocator_type>(allocator)};

    if (length) {
      sycl::buffer<vector3<float>, 1> output_buffer = {output.data(), sycl::range<1>{length}};
      queue
          .submit([&output = output_buffer, &input = *input_buffer,
			       &logger = logger](sycl::handler& command_group) mutable {
		    logger.info("get_debug_vertices(guid const&, allocator&&)") << "Sending " << length
				                                                        << " vertices to GPU.";
            command_group.parallel_for<class push_host_vector_to_global_memory>(
                sycl::range<1>(input.get_count()),
                [input_accessor = input.get_access<mode::read>(command_group),
                 output_accessor = output.get_access<mode::discard_write>(command_group)](
                    sycl::item<1> item) mutable {
                  size_t const id = item.get_linear_id();
                  output_accessor[id] = convert_from_sycl(input_accessor[id]);
                });
          })
          .wait_and_throw();
    }

    return output;
  }
}
